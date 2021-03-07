#include <utility>

#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_

#include "kernel_arg.hpp"

namespace neosycl::sycl::detail {

struct kernel {
  vector_class<KernelArg> args;
  string_class name;
};

}

#endif //SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_HPP_
