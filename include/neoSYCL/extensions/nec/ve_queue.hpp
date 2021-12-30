#ifndef SYCL_INCLUDE_CL_SYCL_NEC_VE_QUEUE_HPP_
#define SYCL_INCLUDE_CL_SYCL_NEC_VE_QUEUE_HPP_
// obsolete
#if 0
#include "ve_offload.h"
#include "CL/SYCL/nec/ve_task.hpp"

namespace neosycl::sycl {

class ve_queue : public queue {
private:
  ve_device dev;
  nec::VEProc proc;

  nec::VEProc proc_create(const string_class &lib_path, int ve_node) {
    struct veo_proc_handle *ve_proc = veo_proc_create(ve_node);
    if (!ve_proc) {
      throw nec::VEException("[VEProc] create ve proc on node: " +
                             std::to_string(ve_node) + " failed..");
    }
    uint64_t handle = veo_load_library(ve_proc, lib_path.c_str());
    DEBUG_INFO("[VEProc] create ve proc: {:#x} and load lib: {} on node: {}",
               (size_t)ve_proc, lib_path, ve_node);
    return nec::VEProc{ve_proc, handle};
  }

  void free_proc(nec::VEProc proc) {
    DEBUG_INFO("[VEProc] release ve proc: {:#x}", (size_t)proc.ve_proc);
    int rt = veo_proc_destroy(proc.ve_proc);
    if (rt != veo_command_state::VEO_COMMAND_OK) {
      DEBUG_INFO("[VEProc] release ve proc: {:#x} failed, return code: {}",
                 (size_t)proc.ve_proc, rt);
      PRINT_ERR("[VEProc] release ve proc failed");
    }
  }

public:
  ve_queue(const string_class &path = nec::DEFAULT_VE_LIB)
      : dev(nec::DEFAULT_VE_NODE), queue() {
    proc = proc_create(path, nec::DEFAULT_VE_NODE);
  }

  ve_queue(const ve_device &dev, const string_class &path = nec::DEFAULT_VE_LIB)
      : dev(dev), queue() {
    proc = proc_create(path, dev.get_node_id());
  }

  detail::Task *build_task() override { return new detail::VETask(proc); }

  virtual ~ve_queue() {
    wait();
    free_proc(proc);
  }
};

} // namespace neosycl::sycl
#endif
#endif // SYCL_INCLUDE_CL_SYCL_NEC_VE_QUEUE_HPP_
