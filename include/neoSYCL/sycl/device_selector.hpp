#ifndef CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_SELECTOR_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_SELECTOR_HPP_

#include "neoSYCL/sycl/device.hpp"
#include "neoSYCL/sycl/detail/platform_info.hpp"

namespace neosycl::sycl {

class device;

class device_selector {
public:
  device_selector() = default;

  device_selector(const device_selector &rhs) = default;

  device_selector &operator=(const device_selector &rhs) = default;

  virtual ~device_selector() = default;

  // defined in device.hpp
  device select_device() const;

  virtual int operator()(const device &device) const = 0;

  virtual shared_ptr_class<detail::platform_info> get_platform_info() const {
    return shared_ptr_class<detail::platform_info>(
        new detail::cpu_platform_info);
  }
};

} // namespace neosycl::sycl

#endif // CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_SELECTOR_HPP_
