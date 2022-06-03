#pragma once
#include "neoSYCL/sycl/detail/container/data_container.hpp"

namespace neosycl::sycl::detail {

struct accessor_info {
  using container_ptr = sycl::device::container_ptr;

  accessor_info(container_ptr arg, access::mode mode)
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

  container_ptr container;
  access::mode mode;
};

} // namespace neosycl::sycl::detail
