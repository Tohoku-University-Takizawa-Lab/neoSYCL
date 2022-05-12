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
  task_handler_ve(const VEProc &p, const VEContext &c) : proc_(p), ctx_(c) {}
  ~task_handler_ve() { /* do nothing */
  }

  void set_capture(const char *name, void *p, size_t sz) override {
    DEBUG_INFO("set capture: %s %#x %#x",name, (size_t)proc_.ve_proc, (size_t)proc_.handle);
    uint64_t devptr = veo_get_sym(proc_.ve_proc, proc_.handle, name);
    if (devptr == 0) {
      PRINT_ERR("veo_get_sym() failed: %s", name);
      throw exception("setup_capture() failed");
    }

    int rt = veo_write_mem(proc_.ve_proc, devptr, p, sz);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_write_mem() failed: %s", name);
      throw exception("setup_capture() failed");
    }
  }

  void set_range(const char* name, range<1>& d) override {
    size_t r[3] = { d[0],1,1 };
    set_range_offset(name, r);
  }
  void set_range(const char* name, range<2>& d) override {
    size_t r[3] = { d[0],d[1],1 };
    set_range_offset(name, r);
  }
  void set_range(const char* name, range<3>& d) override {
    size_t r[3] = { d[0],d[1],d[2] };
    set_range_offset(name, r);
  }

  void set_offset(const char* name, id<1>& i) override {
    size_t o[3] = { i[0],1,1 };
    set_range_offset(name, o);
  }
  void set_offset(const char* name, id<2>& i) override {
    size_t o[3] = { i[0],i[1],1 };
    set_range_offset(name, o);
  }
  void set_offset(const char* name, id<3>& i) override {
    size_t o[3] = { i[0],i[1], i[2] };
    set_range_offset(name, o);
  }

  void set_range_offset (const char* name, size_t r[3]) {
    DEBUG_INFO("set range/offset: %s %lu %lu %lu", name, r[0], r[1], r[2]);
    uint64_t devptr = veo_get_sym(proc_.ve_proc, proc_.handle, name);
    if (devptr == 0) {
      PRINT_ERR("veo_get_sym() failed: %s", name);
      throw exception("setup_range_offset() failed");
    }

    int rt = veo_write_mem(proc_.ve_proc, devptr, r, sizeof(size_t)*3);
    if (rt != VEO_COMMAND_OK) {
      PRINT_ERR("veo_write_mem() failed: %s", name);
      throw exception("setup_range_offset() failed");
    }
  }


  struct veo_args *alloc_veo_args() {
    struct veo_args *argp = veo_args_alloc();
    if (!argp) {
      PRINT_ERR("veo_args_alloc() failed");
      throw exception("alloc_veo_args() failed");
    }
    return argp;
  }

  void single_task(shared_ptr_class<detail::kernel> k,
                   const std::function<void(void)> &func) override {
    DEBUG_INFO("single_task(): %s", k->name.c_str());
    call_kernel_func(k);
  }

  void parallel_for_1d(shared_ptr_class<detail::kernel> k, range<1> r,
                       const std::function<void(id<1>)> &func,
                       id<1> offset) override {
    DEBUG_INFO("parallel_for_1d(): %s", k->name.c_str());
    call_kernel_func(k);
  }

  void parallel_for_2d(shared_ptr_class<detail::kernel> k, range<2> r,
                       const std::function<void(id<2>)> &func,
                       id<2> offset) override {
    DEBUG_INFO("parallel_for_2d(): %s", k->name.c_str());
    call_kernel_func(k);
  }

  void parallel_for_3d(shared_ptr_class<detail::kernel> k, range<3> r,
                       const std::function<void(id<3>)> &func,
                       id<3> offset) override {
    DEBUG_INFO("parallel_for_3d(): %s", k->name.c_str());
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

  void *alloc_mem(container_type d,
                  access::mode mode = access::mode::read) override {
    int index          = find_buf(d);
    bool to_be_updated = (mode != access::mode::read);
    if (index >= 0) {
      bufs_[index].updated = to_be_updated;
      return reinterpret_cast<void *>(bufs_[index].ptr);
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
        mode != access::mode::discard_read_write) {
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

    return reinterpret_cast<void *>(bi.ptr);
  }

  void *get_pointer(container_type d) override {
    int index = find_buf(d);
    if (index < 0)
      return nullptr;
    return reinterpret_cast<void *>(bufs_[index].ptr);
  }

  void copy_back(buf_info &bi) {
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

  void call_kernel_func(shared_ptr_class<detail::kernel> k) {
    for (const detail::accessor_info &acc : k->args) {
      acc.acquire_access();
      alloc_mem(acc.container, acc.mode);
    }
    DEBUG_INFO("-- KENREL EXEC BEGIN --");
    try {
      struct veo_args *argp = alloc_veo_args();
      uint64_t id           = veo_call_async_by_name(ctx_.ve_ctx, proc_.handle,
                                                     k->name.c_str(), argp);
      uint64_t ret_val;
      veo_call_wait_result(ctx_.ve_ctx, id, &ret_val);
      DEBUG_INFO("-- KERNEL EXEC END (ret=%lu) --", ret_val);
      // copy_out(ve_addr_list, k, proc);
      veo_args_free(argp);
    } catch (exception &e) {
      PRINT_ERR("kernel execution failed: %s", e.what());
    }
    for (const detail::accessor_info &acc : k->args) {
      acc.release_access();
    }
  }
};

} // namespace neosycl::sycl::extensions::nec
#endif // NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP
