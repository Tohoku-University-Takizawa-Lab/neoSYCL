#pragma once
#include "neoSYCL/sycl/detail/container/data_container.hpp"

namespace neosycl::sycl::detail {

struct accessor_data {
  using container_ptr = shared_ptr_class<container::DataContainer>;

  accessor_data(container_ptr arg, access::mode mode)
      : data(std::move(arg)), mode(mode) {}

  void acquire_access() const {
    switch (mode) {
    case access::mode::read: {
      data->lock_read();
      break;
    }
    default: {
      data->lock_write();
    }
    }
  }

  void release_access() const {
    switch (mode) {
    case access::mode::read: {
      data->unlock_read();
      break;
    }
    default: {
      data->unlock_write();
    }
    }
  }

  container_ptr data;
  access::mode mode;
};

} // namespace neosycl::sycl::detail
