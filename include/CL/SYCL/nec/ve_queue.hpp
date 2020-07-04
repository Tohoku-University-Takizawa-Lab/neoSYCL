#ifndef SYCL_INCLUDE_CL_SYCL_NEC_VE_QUEUE_HPP_
#define SYCL_INCLUDE_CL_SYCL_NEC_VE_QUEUE_HPP_

#include "CL/SYCL/nec/ve_task.hpp"
#include "CL/SYCL/nec/offload_driver.hpp"

namespace cl::sycl {

class ve_queue : public queue {
 private:
  ve_device dev;
  nec::VEProc proc;

 public:
  ve_queue(const string_class &path = nec::DEFAULT_VE_LIB) : dev(nec::DEFAULT_VE_NODE), queue() {
    proc = nec::proc_create(path, nec::DEFAULT_VE_NODE);
  }

  ve_queue(const ve_device &dev, const string_class &path = nec::DEFAULT_VE_LIB) : dev(dev), queue() {
    proc = nec::proc_create(path, dev.get_node_id());
  }

  detail::Task *build_task() override {
    return new detail::VETask(proc);
  }

  virtual ~ve_queue() {
    wait();
    nec::free_proc(proc);
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_NEC_VE_QUEUE_HPP_
