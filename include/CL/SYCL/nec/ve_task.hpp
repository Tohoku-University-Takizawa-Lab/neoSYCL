#ifndef SYCL_INCLUDE_CL_SYCL_NEC_VE_TASK_HPP_
#define SYCL_INCLUDE_CL_SYCL_NEC_VE_TASK_HPP_

#include "CL/SYCL/nec/ve_kernel.hpp"

namespace cl::sycl::detail {

struct VETask : public Task {
  nec::VEProc proc;

  VETask(const nec::VEProc &proc) : proc(proc) {}

  bool is_cpu() override {
    return false;
  }

  std::shared_ptr<Kernel> get_kernel() override {
    return std::shared_ptr<Kernel>(new VEKernel(args, proc));
  }
};

}

#endif //SYCL_INCLUDE_CL_SYCL_NEC_VE_TASK_HPP_
