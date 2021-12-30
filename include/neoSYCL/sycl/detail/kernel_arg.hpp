#ifndef SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_
#define SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_

#include "neoSYCL/sycl/detail/container/data_container.hpp"
#include "neoSYCL/sycl/access.hpp"

namespace neosycl::sycl::detail {

struct KernelArg {
  KernelArg(std::shared_ptr<detail::container::DataContainer> arg,
            access::mode mode)
      : container(std::move(arg)), mode(mode) {}

  void acquire_access() const {
    switch (mode) {
    case access::mode::read: {
      container->lock_read();
      break;
    }
    default: {
      container->lock_write();
    }
    }
  }

  void release_access() const {
    switch (mode) {
    case access::mode::read: {
      container->unlock_read();
      break;
    }
    default: {
      container->unlock_write();
    }
    }
  }

  std::shared_ptr<detail::container::DataContainer> container;
  access::mode mode;
};

} // namespace neosycl::sycl::detail

#endif // SYCL_INCLUDE_CL_SYCL_KERNEL_KERNEL_ARG_HPP_
