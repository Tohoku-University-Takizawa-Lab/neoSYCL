#pragma once

namespace neosycl::sycl {

class device;

class device_selector {
public:
  device_selector() = default;

  device_selector(const device_selector& rhs) = default;

  device_selector& operator=(const device_selector& rhs) = default;

  virtual ~device_selector() = default;

  virtual device select_device() const = 0;

  virtual int operator()(const device& device) const = 0;
};

} // namespace neosycl::sycl

