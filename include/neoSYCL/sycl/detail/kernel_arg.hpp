#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_

#include "container/data_container.hpp"
#include "../access.hpp"

namespace neosycl::sycl::detail {

class KernelArg {

  KernelArg(std::shared_ptr<detail::container::DataContainer> arg, access::mode mode) :
      container(std::move(arg)), mode(mode) {}

  std::shared_ptr<detail::container::DataContainer> container;
  access::mode mode;

};

}

#endif //SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_
