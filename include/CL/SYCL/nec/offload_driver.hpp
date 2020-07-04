#ifndef SYCL_INCLUDE_CL_SYCL_NEC_VEOFFLOAD_HPP_
#define SYCL_INCLUDE_CL_SYCL_NEC_VEOFFLOAD_HPP_

#include <utility>

#include "ve_offload.h"

namespace cl::sycl::nec {

const int DEFAULT_VE_NODE = 0;
const string_class DEFAULT_VE_LIB = "./kernel.so";

class VEException : public SYCLException {
 private:
  string_class msg;

 public:
  VEException(string_class msg) : msg(std::move(msg)) {}

  const char *what() const noexcept override {
    return msg.c_str();
  }

};

struct VEProc {
  struct veo_proc_handle *ve_proc;
  uint64_t handle;
};

struct VEContext {
  struct veo_thr_ctxt *ve_ctx;
};

VEProc proc_create(const string_class &lib_path, int ve_node) {
  struct veo_proc_handle *ve_proc = veo_proc_create(ve_node);
  if (!ve_proc) {
    throw VEException("[VEProc] create ve proc on node: " + std::to_string(ve_node) + " failed..");
  }
  uint64_t handle = veo_load_library(ve_proc, lib_path.c_str());
  DEBUG_INFO("[VEProc] create ve proc: {:#x} and load lib: {} on node: {}", (size_t) ve_proc, lib_path, ve_node);
  return VEProc{ve_proc, handle};
}

VEContext ctx_create(VEProc proc) {
  struct veo_thr_ctxt *ctx = veo_context_open(proc.ve_proc);
  DEBUG_INFO("[VEContext] create ve context: {:#x}", (size_t) ctx);
  return VEContext{ctx};
}

void free_proc(VEProc proc) {
  DEBUG_INFO("[VEProc] release ve proc: {:#x}", (size_t) proc.ve_proc);
  int rt = veo_proc_destroy(proc.ve_proc);
  if (rt != veo_command_state::VEO_COMMAND_OK) {
    DEBUG_INFO("[VEProc] release ve proc: {:#x} failed, return code: {}", (size_t) proc.ve_proc, rt);
  }
}

void free_ctx(VEContext ctx) {
  DEBUG_INFO("[VEContext] release ve ctx: {:#x}", (size_t) ctx.ve_ctx);
  int rt = veo_context_close(ctx.ve_ctx);
  if (rt != veo_command_state::VEO_COMMAND_OK) {
    DEBUG_INFO("[VEContext] release ve ctx: {:#x} failed, return code: {}", (size_t) ctx.ve_ctx, rt);
  }
}

struct veo_args *create_ve_args() {
  struct veo_args *argp = veo_args_alloc();
  if (!argp) {
    throw VEException("ve args return nullptr");
  }
  return argp;
}

}

#endif //SYCL_INCLUDE_CL_SYCL_NEC_VEOFFLOAD_HPP_
