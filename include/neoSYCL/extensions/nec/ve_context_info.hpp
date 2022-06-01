#ifndef NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_CONTEXT_INFO_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_CONTEXT_INFO_HPP

#include "neoSYCL/extensions/nec/ve_info.hpp"

namespace neosycl::sycl::extensions::nec {

class ve_context_info : public detail::context_info {
  ve_context ctx;

  void free_ctx() {
    if (ctx.ve_ctx == nullptr)
      return;
    DEBUG_INFO("veo_ctxt released: %lx", (size_t)ctx.ve_ctx);
    int rt = veo_context_close(ctx.ve_ctx);
    if (rt != veo_command_state::VEO_COMMAND_OK) {
      PRINT_ERR("veo_context_close() failed: %lx, retval=%d",
                (size_t)ctx.ve_ctx, rt);
    }
    DEBUG_INFO("veo_proc released: %lx", (size_t)ctx.ve_proc);
    rt = veo_proc_destroy(ctx.ve_proc);
    if (rt != veo_command_state::VEO_COMMAND_OK) {
      PRINT_ERR("veo_proc_destroy() failed");
    }
  }

  bool create_ctx(const string_class& lib_path = DEFAULT_VE_LIB,
                  int ve_node                  = DEFAULT_VE_NODE) {
    struct veo_proc_handle* ve_proc = veo_proc_create(ve_node);
    if (!ve_proc) {
      DEBUG_INFO("veo_proc_create(%d) failed", ve_node);
      return false;
    }
    DEBUG_INFO("veo_proc created: %lx", (size_t)ve_proc);

    const char* env = getenv(ENV_VE_KERNEL);
    string_class fn(env ? env : lib_path);
    uint64_t handle = veo_load_library(ve_proc, fn.c_str());
    if (handle == 0) {
      DEBUG_INFO("veo_load_library failed: %s", fn.c_str());
      return false;
    }
    DEBUG_INFO("kernel lib loaded: %lx, %s", (size_t)ve_proc, fn.c_str());

    struct veo_thr_ctxt* c = veo_context_open(ve_proc);
    DEBUG_INFO("veo_ctxt created: %lx", (size_t)c);

    ctx.ve_proc = ve_proc;
    ctx.ve_ctx  = c;
    ctx.handle  = handle;

    return true;
  }

public:
  ve_context_info(device d)
      : detail::context_info(d), ctx{nullptr, nullptr, 0} {
    open();
  }

  ~ve_context_info() { free_ctx(); }

  bool open() override {
    bool rt = create_ctx();
    if (rt) {
      task_handler = task_handler_ptr(new task_handler_ve(ctx));
    }
    DEBUG_INFO("veo_task_handler created %d", rt);
    return rt;
  }
};
} // namespace neosycl::sycl::extensions::nec
#endif
