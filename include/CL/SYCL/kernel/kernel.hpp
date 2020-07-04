#include <utility>

#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_

namespace cl::sycl::detail {

class Task;

struct Kernel {

  vector_class<detail::KernelArg> args;

  Kernel(vector_class<cl::sycl::detail::KernelArg> args) : args(std::move(args)) {}

  virtual void single_task(string_class kernel_name) {

  }

  virtual void parallel_for(const range<1> &r, string_class kernel_name) {

  }

  virtual void parallel_for(const range<2> &r, string_class kernel_name) {

  }

  virtual void parallel_for(const range<3> &r, string_class kernel_name) {

  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_
