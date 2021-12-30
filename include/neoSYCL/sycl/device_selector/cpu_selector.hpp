#ifndef SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_CPU_SELECTOR_HPP_
#define SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_CPU_SELECTOR_HPP_

#include "neoSYCL/sycl/detail/context_info.hpp"

namespace neosycl::sycl {

class cpu_selector : public device_selector {

public:
  int operator()(const device &dev) const override {
    if (dev.is_cpu()) {
      return true;
    }
    return false;
  }

  device select_device() const override {
    return device(
        shared_ptr_class<detail::device_info>(new detail::cpu_device_info()));
  }
};

using default_selector = cpu_selector;
using host_selector    = cpu_selector;

detail::context_info *detail::cpu_device_info::create_context_info() const {
  return new detail::cpu_context_info();
}

} // namespace neosycl::sycl

#endif // SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_CPU_SELECTOR_HPP_
