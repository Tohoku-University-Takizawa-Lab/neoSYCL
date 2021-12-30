#ifndef NEOSYCL_INCLUDE_CL_SYCL_NEC_VE_INFO_HPP_
#define NEOSYCL_INCLUDE_CL_SYCL_NEC_VE_INFO_HPP_

#include "ve_offload.h"

namespace neosycl::sycl::extensions {

const int DEFAULT_VE_NODE         = 0;
const string_class DEFAULT_VE_LIB = "./kernel.so";

struct VEProc {
  struct veo_proc_handle *ve_proc;
  uint64_t handle;
};

struct VEContext {
  struct veo_thr_ctxt *ve_ctx;
};
} // namespace neosycl::sycl::extensions

#endif // NEOSYCL_INCLUDE_CL_SYCL_NEC_VE_INFO_HPP_
