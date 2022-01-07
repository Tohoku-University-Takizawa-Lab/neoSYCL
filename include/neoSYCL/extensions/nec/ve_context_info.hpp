#ifndef NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_CONTEXT_INFO_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_EXTENSIONS_NEC_VE_CONTEXT_INFO_HPP

#include "neoSYCL/extensions/nec/ve_info.hpp"

namespace neosycl::sycl::extensions::nec {

class ve_context_info : public detail::context_info {
  VEProc proc;
  VEContext ctx;

  VEContext create_ctx(VEProc proc) {
    struct veo_thr_ctxt *ctx = veo_context_open(proc.ve_proc);
    DEBUG_INFO("[VEContext] create ve context: {:#x}", (size_t)ctx);
    return VEContext{ctx};
  }

  void free_ctx() {
    DEBUG_INFO("[VEContext] release ve ctx: {:#x}", (size_t)ctx.ve_ctx);
    int rt = veo_context_close(ctx.ve_ctx);
    if (rt != veo_command_state::VEO_COMMAND_OK) {
      DEBUG_INFO("[VEContext] release ve ctx: {:#x} failed, return code: {}",
                 (size_t)ctx.ve_ctx, rt);
      PRINT_ERR("[VEContext] release ve ctx failed");
    }
  }

  VEProc create_proc(const string_class &lib_path = DEFAULT_VE_LIB,
                     int ve_node                  = DEFAULT_VE_NODE) {
    struct veo_proc_handle *ve_proc = veo_proc_create(ve_node);
    if (!ve_proc) {
      DEBUG_INFO("[VEProc] create ve proc on node: %d failed..", ve_node);
      throw ve_exception("[VEProc] create ve proc failed.");
    }
    uint64_t handle = veo_load_library(ve_proc, lib_path.c_str());
    DEBUG_INFO("[VEProc] create ve proc: {:#x} and load lib: {} on node: {}",
               (size_t)ve_proc, lib_path, ve_node);
    return nec::VEProc{ve_proc, handle};
  }

  void free_proc() {
    DEBUG_INFO("[VEProc] release ve proc: {:#x}", (size_t)proc.ve_proc);
    int rt = veo_proc_destroy(proc.ve_proc);
    if (rt != veo_command_state::VEO_COMMAND_OK) {
      DEBUG_INFO("[VEProc] release ve proc: {:#x} failed, return code: {}",
                 (size_t)proc.ve_proc, rt);
      PRINT_ERR("[VEProc] release ve proc failed");
    }
  }

public:
  ve_context_info()
      : detail::context_info(), proc(create_proc()), ctx(create_ctx(proc)) {
    task_handler = handler_type(new task_handler_ve(proc, ctx));
  }
  ~ve_context_info() {
    free_ctx();
    free_proc();
  }
};
} // namespace neosycl::sycl::extensions::nec
#endif
