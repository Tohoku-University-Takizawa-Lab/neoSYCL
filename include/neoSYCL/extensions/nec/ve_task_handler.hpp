#ifndef NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP

#include "neoSYCL/extensions/nec/ve_info.hpp"
#include "neoSYCL/sycl/detail/accessor_info.hpp"
#include "ve_offload.h"

namespace neosycl::sycl::extensions::nec {

class task_handler_ve : public detail::task_handler {
  struct buf_info{
    detail::accessor_info arg;
    uint64_t ptr;
  };
  using buffer_type = std::vector<struct buf_info>;
public:
  task_handler_ve(const VEProc &p, const VEContext &c) : proc(p), ctx(c) {}
  ~task_handler_ve(){
   copy_out();
  }
  struct veo_args *create_ve_args() {
    struct veo_args *argp = veo_args_alloc();
    if (!argp) {
      throw exception("ve args return nullptr");
    }
    return argp;
  }

  void copy_in(struct veo_args *argp,
                                 shared_ptr_class<detail::kernel> k,
                                 VEProc proc) {
    int i, j;

    for (i = 0; i < k->args.size(); i++) {
      detail::accessor_info arg = k->args[i];
      for(j = 0; j< bufs.size(); j++){
        if(arg.container->get_raw_ptr() == bufs[j].arg.container->get_raw_ptr())
          break;
      }
      if(j!=bufs.size()){
        veo_args_set_i64(argp, i, bufs[j].ptr);
        continue;
      }
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
      //ve_addr_list.push_back(ve_addr_int);
      buf_info bi{arg,ve_addr_int};
      bufs.push_back(bi);

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
    return;
  }

  void copy_out() {
    for (int i = 0; i < bufs.size(); i++) {
      detail::accessor_info arg = bufs[i].arg;
      size_t size_in_byte   = arg.container->get_size();
      uint64_t device_ptr   = bufs[i].ptr;
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
    for (const detail::accessor_info &arg : k->args) {
      arg.acquire_access();
    }
    DEBUG_INFO("execute single %d kernel, name: %s\n", type(), k->name.c_str());

    DEBUG_INFO("[VEKernel] single task: {}", k->name.c_str());

    veo_args *argp = create_ve_args();
    DEBUG_INFO("[VEKernel] create ve args: {:#x}", (size_t)argp);

    try {

      copy_in(argp, k, proc);
      DEBUG_INFO("[VEKernel] invoke ve func: {}", k->name.c_str());
      uint64_t id = veo_call_async_by_name(ctx.ve_ctx, proc.handle,
                                           k->name.c_str(), argp);
      uint64_t ret_val;
      veo_call_wait_result(ctx.ve_ctx, id, &ret_val);
      DEBUG_INFO("[VEKernel] ve func finished, id: {}, ret val: {}", id,
                 ret_val);
      //copy_out(ve_addr_list, k, proc);

    } catch (exception &e) {
      std::cerr << "[VEKernel] kernel invoke failed, error message: "
                << e.what() << std::endl;
    }

    veo_args_free(argp);

    for (const detail::accessor_info &arg : k->args) {
      arg.release_access();
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

    DEBUG_INFO("[VEKernel] parallel task: {}", k->name.c_str());

    veo_args *argp = create_ve_args();
    DEBUG_INFO("[VEKernel] create ve args: {:#x}", (size_t)argp);

    try {

      copy_in(argp, k, proc);
      set_arg_for_range(k->args, argp, r);
      DEBUG_INFO("[VEKernel] invoke ve func: {}", k->name.c_str());
      uint64_t id = veo_call_async_by_name(ctx.ve_ctx, proc.handle,
                                           k->name.c_str(), argp);
      uint64_t ret_val;
      veo_call_wait_result(ctx.ve_ctx, id, &ret_val);
      DEBUG_INFO("[VEKernel] ve func finished, id: {}, ret val: {}", id,
                 ret_val);
      //copy_out(ve_addr_list, k, proc);

    } catch (exception &e) {
      std::cerr << "[VEKernel] kernel invoke failed, error message: "
                << e.what() << std::endl;
    }
    veo_args_free(argp);
    for (const detail::accessor_info &arg : k->args) {
      arg.release_access();
    }
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
  buffer_type bufs;
};

} // namespace neosycl::sycl::extensions::nec
#endif // NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_TASK_HANDLER_HPP
