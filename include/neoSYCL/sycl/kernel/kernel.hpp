#include <utility>

#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_

namespace neosycl::sycl::detail {

class Task;

struct Kernel {

  vector_class<detail::KernelArg> args;
  string_class kernel_name;

  Kernel(vector_class<detail::KernelArg> args, string_class name) :
      args(std::move(args)), kernel_name(std::move(name)) {}

  virtual void single_task() {

  }

  virtual void parallel_for(const range<1> &r) {

  }

  virtual void parallel_for(const range<2> &r) {

  }

  virtual void parallel_for(const range<3> &r) {

  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_
