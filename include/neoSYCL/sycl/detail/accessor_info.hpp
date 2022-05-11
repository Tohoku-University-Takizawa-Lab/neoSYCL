#ifndef SYCL_INCLUDE_CL_SYCL_ACCESSOR_INFO_HPP_
#define SYCL_INCLUDE_CL_SYCL_ACCESSOR_INFO_HPP_
#include "neoSYCL/sycl/detail/container/data_container.hpp"
#include "neoSYCL/sycl/access.hpp"

namespace neosycl::sycl::detail {

struct accessor_info {
  using container_type = std::shared_ptr<detail::container::DataContainer>;

  accessor_info(container_type arg, access::mode mode)
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

  container_type container;
  access::mode mode;
};

} // namespace neosycl::sycl::detail

#endif // SYCL_INCLUDE_CL_SYCL_ACCESSOR_INFO_HPP_
