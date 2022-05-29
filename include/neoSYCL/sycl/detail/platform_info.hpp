#pragma once

#include "neoSYCL/sycl/detail/device_info.hpp"
#include "neoSYCL/sycl/detail/device_type.hpp"

namespace neosycl::sycl {
class device;

namespace detail {
class platform_info {
public:
  platform_info(device host_device);

  virtual bool is_host() = 0;

  virtual bool has_extension(const string_class& extension) = 0;

  vector_class<device> list_devices() { return dev_; }

  vector_class<device> dev_;
};

class host_platform_info : public platform_info {
public:
  host_platform_info(device host_device);

  bool is_host() override { return true; }
  bool has_extension(const string_class& extension) override { return false; }
};

using default_platform_info = host_platform_info;

} // namespace detail
} // namespace neosycl::sycl
