#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_TASK_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_TASK_HPP_

#include <utility>

#include "kernel.hpp"

namespace neosycl::sycl::detail {

struct Task {
  vector_class<detail::KernelArg> args;

  Task() {}

  void add_arg(const int &index, const detail::KernelArg &arg) {
    if (args.size() <= index) {
      args.resize(index + 1);
    }
    args[index] = arg;
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_KERNEL_TASK_HPP_
