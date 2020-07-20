#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_TASK_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_TASK_HPP_

#include <utility>

#include "CL/SYCL/kernel/kernel_arg.hpp"
#include "CL/SYCL/kernel/kernel.hpp"

namespace cl::sycl::detail {

struct Task {
  vector_class<detail::KernelArg> args;

  Task() {}

  void add_arg(const detail::KernelArg &arg) {
    args.push_back(arg);
  }

  virtual bool is_cpu() {
    return true;
  }

  virtual std::shared_ptr<Kernel> get_kernel(string_class name) {
    return std::shared_ptr<Kernel>(new Kernel(args, name));
  };
};

}

#endif //SYCL_INCLUDE_CL_SYCL_KERNEL_TASK_HPP_
