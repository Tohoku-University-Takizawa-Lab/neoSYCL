#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_

#include "../container/data_container.hpp"

namespace neosycl::sycl::detail {

struct KernelArg {

  std::shared_ptr<detail::DataContainer> container;
  access::mode mode;

  KernelArg(std::shared_ptr<detail::DataContainer> arg, access::mode mode) : container(std::move(arg)), mode(mode) {}

};

}

#endif //SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_
