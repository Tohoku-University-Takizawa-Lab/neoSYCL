#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_TASK_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_TASK_HPP_

#include "CL/SYCL/kernel/kernel_arg.hpp"
#include "CL/SYCL/kernel/kernel.hpp"

namespace cl::sycl::detail {

struct Task {
  vector_class<detail::KernelArg> args;
  string_class name;

  void add_arg(const detail::KernelArg &arg) {
    args.push_back(arg);
  }

  virtual bool is_cpu() {
    return true;
  }

  virtual std::shared_ptr<Kernel> get_kernel() {
    return std::shared_ptr<Kernel>(new Kernel(args));
  };
};

}

#endif //SYCL_INCLUDE_CL_SYCL_KERNEL_TASK_HPP_