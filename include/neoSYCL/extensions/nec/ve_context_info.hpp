#ifndef NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_CONTEXT_INFO_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_CONTEXT_INFO_HPP

#include "neoSYCL/extensions/nec/ve_info.hpp"

namespace neosycl::sycl::extensions::nec {

class ve_context_info : public detail::context_info {
  VEProc proc;
  VEContext ctx;

  VEContext create_ctx(VEProc proc) {
    struct veo_thr_ctxt* c = veo_context_open(proc.ve_proc);
    DEBUG_INFO("veo_ctxt created: %lx", (size_t)c);
    return VEContext{c};
  }

  void free_ctx() {
    DEBUG_INFO("veo_ctxt released: %lx", (size_t)ctx.ve_ctx);
    int rt = veo_context_close(ctx.ve_ctx);
    if (rt != veo_command_state::VEO_COMMAND_OK) {
      PRINT_ERR("veo_context_close() failed: %lx, retval=%d",
                (size_t)ctx.ve_ctx, rt);
    }
  }

  VEProc create_proc(const string_class& lib_path = DEFAULT_VE_LIB,
                     int ve_node                  = DEFAULT_VE_NODE) {
    struct veo_proc_handle* ve_proc = veo_proc_create(ve_node);
    if (!ve_proc) {
      PRINT_ERR("veo_proc_create(%d) failed", ve_node);
      throw ve_exception("create_proc() failed");
    }
    DEBUG_INFO("veo_proc created: %lx", (size_t)ve_proc);

    const char* env = getenv(ENV_VE_KERNEL);
    string_class fn(env ? env : lib_path);
    uint64_t handle = veo_load_library(ve_proc, fn.c_str());
    if (handle == 0) {
      PRINT_ERR("veo_load_library failed: %s", fn.c_str());
      throw ve_exception("create_proc failed");
    }
    DEBUG_INFO("kernel lib loaded: %lx, %s", (size_t)ve_proc, fn.c_str());
    return nec::VEProc{ve_proc, handle};
  }

  void free_proc() {
    DEBUG_INFO("veo_proc released: %lx", (size_t)proc.ve_proc);
    int rt = veo_proc_destroy(proc.ve_proc);
    if (rt != veo_command_state::VEO_COMMAND_OK) {
      PRINT_ERR("veo_proc_destroy() failed");
    }
  }

public:
  ve_context_info(device d)
      : detail::context_info(d), proc(create_proc()), ctx(create_ctx(proc)) {
    task_handler = handler_type(new task_handler_ve(proc, ctx));
  }
  ~ve_context_info() {
    free_ctx();
    free_proc();
  }
};
} // namespace neosycl::sycl::extensions::nec
#endif
