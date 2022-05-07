#ifndef NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP

#include "neoSYCL/extensions/nec/ve_info.hpp"
#include "neoSYCL/sycl/detail/accessor_info.hpp"
#include "ve_offload.h"

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
  ~task_handler_ve() { free_mem(); }

  struct veo_args *alloc_veo_args() {
    struct veo_args *argp = veo_args_alloc();
    if (!argp) {
      throw exception("ve args return nullptr");
    }
    DEBUG_INFO("[VEKernel] create ve args: %#x", (size_t)argp);
    return argp;
  }

  struct veo_args *create_ve_args(shared_ptr_class<detail::kernel> k) {
    struct veo_args *argp = alloc_veo_args();

    for (int i = 0; i < k->args.size(); i++) {
      detail::accessor_info acc = k->args[i];

      void *ve_addr        = alloc_mem(acc.container, acc.mode);
      uint64_t ve_addr_int = reinterpret_cast<uint64_t>(ve_addr);
      veo_args_set_i64(argp, i, ve_addr_int);
    }
    return argp;
  }

  void single_task(shared_ptr_class<detail::kernel> k,
                   const std::function<void(void)> &func) override {
    for (const detail::accessor_info &acc : k->args) {
      acc.acquire_access();
    }
    DEBUG_INFO("execute single %d kernel, name: %s\n", type(), k->name.c_str());
    DEBUG_INFO("[VEKernel] single task: %s", k->name.c_str());
    try {
      struct veo_args *argp = create_ve_args(k);
      DEBUG_INFO("[VEKernel] invoke ve func: %s", k->name.c_str());
      uint64_t id = veo_call_async_by_name(ctx_.ve_ctx, proc_.handle,
                                           k->name.c_str(), argp);
      uint64_t ret_val;
      veo_call_wait_result(ctx_.ve_ctx, id, &ret_val);
      DEBUG_INFO("[VEKernel] ve func finished, id: %lu, ret val: %lu", id,
                 ret_val);
      // copy_out(ve_addr_list, k, proc);
      veo_args_free(argp);
    } catch (exception &e) {
      std::cerr << "[VEKernel] kernel invoke failed, error message: "
                << e.what() << std::endl;
    }
    for (const detail::accessor_info &acc : k->args) {
      acc.release_access();
    }
  }

  void set_arg_for_range(const vector_class<detail::accessor_info> &args,
                         struct veo_args *argp, const range<1> &r) {
    int index = args.size();
    veo_args_set_i64(argp, index, r.size());
    veo_args_set_i64(argp, index + 1, 1);
  }

  void parallel_for_1d(shared_ptr_class<detail::kernel> k, range<1> r,
                       const std::function<void(id<1>)> &func,
                       id<1> offset) override {
    for (const detail::accessor_info &arg : k->args) {
      arg.acquire_access();
    }
    DEBUG_INFO("execute parallel<1> %d kernel, name: %s\n", type(),
               k->name.c_str());
    DEBUG_INFO("[VEKernel] parallel task: %s", k->name.c_str());
    try {
      struct veo_args *argp = create_ve_args(k);
      DEBUG_INFO("[VEKernel] create ve args: %#x", (size_t)argp);
      set_arg_for_range(k->args, argp, r);
      DEBUG_INFO("[VEKernel] invoke ve func: %s", k->name.c_str());
      uint64_t id = veo_call_async_by_name(ctx_.ve_ctx, proc_.handle,
                                           k->name.c_str(), argp);
      uint64_t ret_val;
      veo_call_wait_result(ctx_.ve_ctx, id, &ret_val);
      DEBUG_INFO("[VEKernel] ve func finished, id: %lu, ret val: %lu", id,
                 ret_val);
      // copy_out(ve_addr_list, k, proc);
      veo_args_free(argp);
    } catch (exception &e) {
      PRINT_ERR("[VEKernel] kernel invoke failed");
      throw exception("kernel invocation error");
    }

    for (const detail::accessor_info &acc : k->args) {
      acc.release_access();
    }
  }

  void parallel_for_2d(shared_ptr_class<detail::kernel> k, range<2> r,
                       const std::function<void(id<2>)> &func,
                       id<2> offset) override {
    throw exception("not implemented");
  }

  void parallel_for_3d(shared_ptr_class<detail::kernel> k, range<3> r,
                       const std::function<void(id<3>)> &func,
                       id<3> offset) override {
    throw exception("not implemented");
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

  void free_mem(container_type d) {
    int index = find_buf(d);
    if (index < 0)
      return;
    copy_back(bufs_[index]);
    uint64_t device_ptr = bufs_[index].ptr;

    int rt = veo_free_mem(proc_.ve_proc, device_ptr);
    if (rt != VEO_COMMAND_OK) {
      DEBUG_INFO("[VEProc] free ve memory failed, size: %lu, return code: %d",
                 bufs_[index].buf->get_size(), rt);
      PRINT_ERR("[VEProc] free ve memory failed");
      throw exception("VE free memory return error");
    }
    bufs_.erase(bufs_.begin() + index);
  }

  void *alloc_mem(container_type d, access::mode mode = access::mode::read) {
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
      DEBUG_INFO("[VEProc] allocate VE memory size: %lu failed, return code: %d",
                 size_in_byte, rt);
      PRINT_ERR("[VEProc] allocate VE memory failed");
      throw exception("VE allocate return error");
    }
    DEBUG_INFO("[VEKernel] allocate ve memory, size: %lu, ve address: %#x",
               size_in_byte, ve_addr_int);
    buf_info bi{d, ve_addr_int, to_be_updated};
    bufs_.push_back(bi);

    if (mode != access::mode::discard_write &&
        mode != access::mode::discard_read_write) {
      DEBUG_INFO("[VEKernel] do copy to ve memory for arg, device address: "
                 "%#x, size: %lu, host address: %#x",
                 (size_t)ve_addr_int, size_in_byte, (size_t)d->get_raw_ptr());
      rt = veo_write_mem(proc_.ve_proc, ve_addr_int, d->get_raw_ptr(),
                         size_in_byte);
      if (rt != VEO_COMMAND_OK) {
        DEBUG_INFO(
            "[VEProc] copy to ve memory failed, size: %lu, return code: %d",
            size_in_byte, rt);
        PRINT_ERR("[VEProc] copy to ve memory failed");
        throw exception("VE copy return error");
      }
    }

    return reinterpret_cast<void *>(bi.ptr);
  }

  void *get_pointer(container_type d) {
    int index = find_buf(d);
    if (index < 0)
      return nullptr;
    return reinterpret_cast<void *>(bufs_[index].ptr);
  }

  void copy_back(buf_info &bi) {
    if (bi.updated) {
      size_t size_in_byte = bi.buf->get_size();
      uint64_t device_ptr = bi.ptr;
      DEBUG_INFO("[VEKernel] copy from ve memory, device address: %#x, "
                 "size: %lu, host address: %#x",
                 (size_t)device_ptr, size_in_byte,
                 (size_t)bi.buf->get_raw_ptr());
      // do copy
      int rt = veo_read_mem(proc_.ve_proc, bi.buf->get_raw_ptr(), device_ptr,
                            size_in_byte);
      if (rt != veo_command_state::VEO_COMMAND_OK) {
        DEBUG_INFO(
            "[VEProc] copy from ve memory failed, size: %lu, return code: %d",
            size_in_byte, rt);
        PRINT_ERR("[VEProc] copy from ve memory failed");
        throw exception("VE copy return error");
      }
      bi.updated = false;
    }
  }

  void copy_back() {
    for (int i = 0; i < bufs_.size(); i++) {
      copy_back(bufs_[i]);
    }
  }

private:
  VEContext ctx_;
  VEProc proc_;
  buffer_type bufs_;
};

} // namespace neosycl::sycl::extensions::nec
#endif // NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP
