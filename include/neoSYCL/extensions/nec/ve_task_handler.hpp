#ifndef NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP

namespace neosycl::sycl::extensions::nec {
class task_handler_ve : public detail::task_handler {
  struct buf_info {
    container_type buf;
    uint64_t ptr;
    bool updated;
  };
  using buffer_type = std::vector<struct buf_info>;

public:
  task_handler_ve(const VEProc& p, const VEContext& c) : proc_(p), ctx_(c) {
    argp_ = alloc_veo_args();
  }
  ~task_handler_ve() { veo_args_free(argp_); }

  kernel* create_kernel(const char* s) override {
    kernel_info_ve* ki = new kernel_info_ve(s);

    std::string oname = std::string("__") + s + "_obj__";

    ki->func_ = veo_get_sym(proc_.ve_proc, proc_.handle, s);
    ki->capt_ = veo_get_sym(proc_.ve_proc, proc_.handle, oname.c_str());

    if (ki->func_ == 0 || ki->capt_ == 0) {
      PRINT_ERR("veo_get_sym() failed: %s", s);
      throw exception("create_kernel() failed");
    }
    return new kernel(ki);
  }

  void set_capture(shared_ptr_class<kernel> k, void* p, size_t sz) override {
    DEBUG_INFO("set capture: %s %#x %#x", k->get_name(), (size_t)proc_.ve_proc,
               (size_t)proc_.handle);

    kernel::info_type ki = k->get_kernel_info();
    shared_ptr_class<kernel_info_ve> kiv =
        std::dynamic_pointer_cast<kernel_info_ve>(ki);
    if (kiv == nullptr) {
      PRINT_ERR("invalid kernel_info: %#x", ki.get());
      throw exception("set_capture() failed");
    }

    DEBUG_INFO("set capture: %#x %#x %#x", (size_t)proc_.ve_proc, (size_t)p,
               sz);
    int rt = veo_write_mem(proc_.ve_proc, kiv->capt_, p, sz);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_write_mem() failed: %s %d", k->get_name(), rt);
      throw exception("setup_capture() failed");
    }
  }

  void set_range(shared_ptr_class<kernel> k, size_t r[6]) {
    kernel::info_type ki = k->get_kernel_info();
    shared_ptr_class<kernel_info_ve> kiv =
        std::dynamic_pointer_cast<kernel_info_ve>(ki);
    if (kiv == nullptr) {
      PRINT_ERR("invalid kernel_info: %#x", ki.get());
      throw exception("set_range() failed");
    }
    DEBUG_INFO("range : %s %lu %lu %lu", k->get_name(), r[0], r[1], r[2]);
    DEBUG_INFO("offset: %s %lu %lu %lu", k->get_name(), r[3], r[4], r[5]);

    if (kiv->rnge_ == 0) {
      // this is the first call and the pointer is not set yet.
      std::string rname = std::string("__") + k->get_name() + "_range__";

      kiv->rnge_ = veo_get_sym(proc_.ve_proc, proc_.handle, rname.c_str());
      if (kiv->rnge_ == 0) {
        PRINT_ERR("veo_get_sym() failed: %s", rname.c_str());
        throw exception("set_range() failed");
      }
    }

    int rt = veo_write_mem(proc_.ve_proc, kiv->rnge_, r, sizeof(size_t) * 6);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_write_mem() failed: %s", k->get_name());
      throw exception("setup_range() failed");
    }
  }

  struct veo_args* alloc_veo_args() {
    struct veo_args* argp = veo_args_alloc();
    if (!argp) {
      PRINT_ERR("veo_args_alloc() failed");
      throw exception("alloc_veo_args() failed");
    }
    return argp;
  }

  void single_task(shared_ptr_class<kernel> k,
                   const std::function<void(void)>& func) override {
    DEBUG_INFO("single_task(): %s", k->get_name());
    call_kernel_func(k);
  }

  void parallel_for_1d(shared_ptr_class<kernel> k, range<1> r,
                       const std::function<void(id<1>)>& func,
                       id<1> offset) override {
    DEBUG_INFO("parallel_for_1d(): %s", k->get_name());
    call_kernel_func(k);
  }

  void parallel_for_2d(shared_ptr_class<kernel> k, range<2> r,
                       const std::function<void(id<2>)>& func,
                       id<2> offset) override {
    DEBUG_INFO("parallel_for_2d(): %s", k->get_name());
    call_kernel_func(k);
  }

  void parallel_for_3d(shared_ptr_class<kernel> k, range<3> r,
                       const std::function<void(id<3>)>& func,
                       id<3> offset) override {
    DEBUG_INFO("parallel_for_3d(): %s", k->get_name());
    call_kernel_func(k);
  }

  detail::SUPPORT_PLATFORM_TYPE type() override { return detail::VE; }

  int find_buf(container_type d) {
    for (int j = 0; j < bufs_.size(); j++) {
      if (d->get_raw_ptr() == bufs_[j].buf->get_raw_ptr()) {
        return j;
      }
    }
    return -1;
  }

  void free_mem() {
    while (bufs_.size() > 0) {
      free_mem(bufs_[0].buf);
    }
  }

  void free_mem(container_type d) override {
    int index = find_buf(d);
    if (index < 0)
      return;
    copy_back(bufs_[index]);
    uint64_t device_ptr = bufs_[index].ptr;

    int rt = veo_free_mem(proc_.ve_proc, device_ptr);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_free_mem() failed: return code=%d", rt);
      throw exception("free_mem() failed");
    }
    bufs_.erase(bufs_.begin() + index);
  }

  void* alloc_mem(container_type d,
                  access::mode mode = access::mode::read) override {
    int index          = find_buf(d);
    bool to_be_updated = (mode != access::mode::read);
    if (index >= 0) {
      bufs_[index].updated = to_be_updated;
      // return reinterpret_cast<void*>(bufs_[index].ptr);
      return nullptr;
    }

    size_t size_in_byte = d->get_size();
    uint64_t ve_addr_int;

    int rt = veo_alloc_mem(proc_.ve_proc, &ve_addr_int, size_in_byte);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_alloc_mem() failed: return code=%d", rt);
      throw exception("alloc_mem() failed");
    }

    DEBUG_INFO("memory alloc: vaddr=%#x, size=%lu", ve_addr_int, size_in_byte);
    buf_info bi{d, ve_addr_int, to_be_updated};
    bufs_.push_back(bi);

    if (mode != access::mode::discard_write &&
        mode != access::mode::discard_read_write)
    {
      DEBUG_INFO("memory copy (h2v): "
                 "vaddr=%#x, haddr=%#x, size=%lu",
                 (size_t)ve_addr_int, (size_t)d->get_raw_ptr(), size_in_byte);
      rt = veo_write_mem(proc_.ve_proc, ve_addr_int, d->get_raw_ptr(),
                         size_in_byte);
      if (rt != VEO_COMMAND_OK) {
        PRINT_ERR("veo_write_mem() failed");
        throw exception("alloc_mem() failed");
      }
    }

    return reinterpret_cast<void*>(bi.ptr);
  }

  void* get_pointer(container_type d) override {
    int index = find_buf(d);
    if (index < 0)
      return nullptr;
    return reinterpret_cast<void*>(bufs_[index].ptr);
  }

  void copy_back(buf_info& bi) {
    if (bi.updated) {
      size_t size_in_byte = bi.buf->get_size();
      uint64_t device_ptr = bi.ptr;
      DEBUG_INFO("memory copy (v2h): "
                 "vaddr=%#x, haddr=%#x, size=%lu",
                 (size_t)device_ptr, (size_t)bi.buf->get_raw_ptr(),
                 size_in_byte);
      // do copy
      int rt = veo_read_mem(proc_.ve_proc, bi.buf->get_raw_ptr(), device_ptr,
                            size_in_byte);
      if (rt != veo_command_state::VEO_COMMAND_OK) {
        PRINT_ERR("veo_read_mem() failed");
        throw exception("copy_back() failed");
      }
    }
  }

  void copy_back() override {
    for (int i = 0; i < bufs_.size(); i++) {
      copy_back(bufs_[i]);
    }
  }

private:
  VEContext ctx_;
  VEProc proc_;
  buffer_type bufs_;
  struct veo_args* argp_;

  void call_kernel_func(shared_ptr_class<kernel> k) {
    kernel::info_type ki = k->get_kernel_info();
    shared_ptr_class<kernel_info_ve> kiv =
        std::dynamic_pointer_cast<kernel_info_ve>(ki);
    if (kiv == nullptr) {
      PRINT_ERR("invalid kernel_info: %#x", ki.get());
      throw exception("set_capture() failed");
    }

    for (const detail::accessor_info& acc : k->get_acc()) {
      acc.acquire_access();
      alloc_mem(acc.container, acc.mode);
    }
    DEBUG_INFO("-- KENREL EXEC BEGIN --");
    try {
      uint64_t ret_val;
      uint64_t id = veo_call_async(ctx_.ve_ctx, kiv->func_, argp_);
      veo_call_wait_result(ctx_.ve_ctx, id, &ret_val);
      DEBUG_INFO("-- KERNEL EXEC END (ret=%lu) --", ret_val);
      // copy_out(ve_addr_list, k, proc);
    }
    catch (exception& e) {
      PRINT_ERR("kernel execution failed: %s", e.what());
    }
    for (const detail::accessor_info& acc : k->get_acc()) {
      acc.release_access();
    }
  }
};

} // namespace neosycl::sycl::extensions::nec
#endif // NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP
