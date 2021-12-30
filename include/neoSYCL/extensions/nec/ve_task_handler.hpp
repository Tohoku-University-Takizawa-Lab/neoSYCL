#ifndef NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP

#include "neoSYCL/extensions/nec/ve_info.hpp"
#include "neoSYCL/sycl/detail/kernel_arg.hpp"
#include "ve_offload.h"

namespace neosycl::sycl::extensions::nec {

class task_handler_ve : public detail::task_handler {

public:
  task_handler_ve(const VEProc &p, const VEContext &c) : proc(p), ctx(c) {}

  struct veo_args *create_ve_args() {
    struct veo_args *argp = veo_args_alloc();
    if (!argp) {
      throw exception("ve args return nullptr");
    }
    return argp;
  }
  
  vector_class<uint64_t> copy_in(struct veo_args *argp,
                                 shared_ptr_class<detail::kernel> k,
                                 VEProc proc) {
    vector_class<uint64_t> ve_addr_list;

    for (int i = 0; i < k->args.size(); i++) {
      detail::KernelArg arg = k->args[i];
      size_t size_in_byte   = arg.container->get_size();

      uint64_t ve_addr_int;
      int rt = veo_alloc_mem(proc.ve_proc, &ve_addr_int, size_in_byte);
      if (rt != veo_command_state::VEO_COMMAND_OK) {
        DEBUG_INFO(
            "[VEProc] allocate VE memory size: {} failed, return code: {}",
            size_in_byte, rt);
        PRINT_ERR("[VEProc] allocate VE memory failed");
        throw exception("VE allocate return error");
      }
      ve_addr_list.push_back(ve_addr_int);

      DEBUG_INFO("[VEKernel] allocate ve memory, size: {}, ve address: {:#x}",
                 size_in_byte, ve_addr_int);

      if (arg.mode != access::mode::write) {
        DEBUG_INFO("[VEKernel] do copy to ve memory for arg, device address: "
                   "{:#x}, size: {}, host address: {:#x}",
                   (size_t)ve_addr_int, size_in_byte,
                   (size_t)arg.container->get_raw_ptr());
        rt = veo_write_mem(proc.ve_proc, ve_addr_int,
                           arg.container->get_raw_ptr(), size_in_byte);
        if (rt != veo_command_state::VEO_COMMAND_OK) {
          DEBUG_INFO(
              "[VEProc] copy to ve memory failed, size: {}, return code: {}",
              size_in_byte, rt);
          PRINT_ERR("[VEProc] copy to ve memory failed");
          throw exception("VE copy return error");
        }
      }
      veo_args_set_i64(argp, i, ve_addr_int);
    }
    return ve_addr_list;
  }

  void copy_out(vector_class<uint64_t> ve_addr_list,
                shared_ptr_class<detail::kernel> k, VEProc proc) {
    for (int i = 0; i < k->args.size(); i++) {
      detail::KernelArg arg = k->args[i];
      size_t size_in_byte   = arg.container->get_size();
      uint64_t device_ptr   = ve_addr_list[i];
      if (arg.mode != access::mode::read) {
        DEBUG_INFO("[VEKernel] copy from ve memory, device address: {:#x}, "
                   "size: {}, host address: {:#x}",
                   (size_t)device_ptr, size_in_byte,
                   (size_t)arg.container->get_raw_ptr());
        // do copy
        int rt = veo_read_mem(proc.ve_proc, arg.container->get_raw_ptr(),
                              device_ptr, size_in_byte);
        if (rt != veo_command_state::VEO_COMMAND_OK) {
          DEBUG_INFO(
              "[VEProc] copy from ve memory failed, size: {}, return code: {}",
              size_in_byte, rt);
          PRINT_ERR("[VEProc] copy from ve memory failed");
          throw exception("VE copy return error");
        }
      }
      int rt = veo_free_mem(proc.ve_proc, device_ptr);
      if (rt != veo_command_state::VEO_COMMAND_OK) {
        DEBUG_INFO("[VEProc] free ve memory failed, size: {}, return code: {}",
                   size_in_byte, rt);
        PRINT_ERR("[VEProc] free ve memory failed");
        throw exception("VE free memory return error");
      }
    }
  }

  void single_task(shared_ptr_class<detail::kernel> k,
                   const std::function<void(void)> &func) override {
    for (const detail::KernelArg &arg : k->args) {
      arg.acquire_access();
    }
    DEBUG_INFO("execute single %d kernel, name: %s\n", type(), k->name.c_str());

    DEBUG_INFO("[VEKernel] single task: {}", k->name.c_str());

    veo_args *argp = create_ve_args();
    DEBUG_INFO("[VEKernel] create ve args: {:#x}", (size_t)argp);

    try {

      vector_class<uint64_t> ve_addr_list = copy_in(argp, k, proc);
      DEBUG_INFO("[VEKernel] invoke ve func: {}", k->name.c_str());
      uint64_t id = veo_call_async_by_name(ctx.ve_ctx, proc.handle,
                                           k->name.c_str(), argp);
      uint64_t ret_val;
      veo_call_wait_result(ctx.ve_ctx, id, &ret_val);
      DEBUG_INFO("[VEKernel] ve func finished, id: {}, ret val: {}", id,
                 ret_val);
      copy_out(ve_addr_list, k, proc);

    } catch (exception &e) {
      std::cerr << "[VEKernel] kernel invoke failed, error message: "
                << e.what() << std::endl;
    }

    veo_args_free(argp);

    for (const detail::KernelArg &arg : k->args) {
      arg.release_access();
    }
  }

  void parallel_for_1d(shared_ptr_class<detail::kernel> k, range<1> r,
                       const std::function<void(id<1>)> &func,
                       id<1> offset) override {
    throw exception("not implemented");
  };

  void parallel_for_2d(shared_ptr_class<detail::kernel> k, range<2> r,
                       const std::function<void(id<2>)> &func,
                       id<2> offset) override {
    throw exception("not implemented");
  };

  void parallel_for_3d(shared_ptr_class<detail::kernel> k, range<3> r,
                       const std::function<void(id<3>)> &func,
                       id<3> offset) override {
    throw exception("not implemented");
  };

  detail::SUPPORT_PLATFORM_TYPE type() override { return detail::VE; }

private:
  VEContext ctx;
  VEProc proc;
};

} // namespace neosycl::sycl::extensions::nec
#endif // NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP
