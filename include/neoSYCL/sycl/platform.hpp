#pragma once
#include "neoSYCL/sycl/info/device_type.hpp"
#include "neoSYCL/sycl/info/platform.hpp"
#include "neoSYCL/sycl/info/device.hpp"
#include "neoSYCL/sycl/info/param_traits.hpp"

namespace neosycl::sycl {

namespace detail {
class platform_impl;
};

class device;

///////////////////////////////////////////////////////////////////////////////
class platform {
  friend class initial_platform_builder;

public:
  platform(const platform& rhs) = default;
  platform(platform&& rhs)      = default;
  ~platform()                   = default;

  platform& operator=(const platform& rhs) = default;
  platform& operator=(platform&& rhs) = default;

  friend bool operator==(const platform& lhs, const platform& rhs);
  friend bool operator!=(const platform& lhs, const platform& rhs);

  // get a static platform object by default (= REGISTERED[0])
  // explicit platform() { *this = get_default_platform(); }
  explicit platform() : impl_(nullptr) {}

  explicit platform(cl_platform_id platformID) {
    throw unimplemented();
  }

  explicit platform(const device_selector& deviceSelector);

  /* -- common interface members -- */
  /* platform is not associated with OpenCL => 0  */
  cl_platform_id get() const {
    return 0;
  }

  vector_class<device>
      get_devices(info::device_type = info::device_type::all) const;

  template <info::platform param>
  typename info::param_traits<info::platform, param>::return_type
  get_info() const {
    throw unimplemented();
  }

  bool has_extension(const string_class& extension) const;

  bool is_host() const;

  static vector_class<platform> get_platforms();
  static platform get_default_platform();
  static platform register_all_devices();

private:
  shared_ptr_class<detail::platform_impl> impl_;

  // INTERNAL USE ONLY: create the default platform
  static vector_class<platform> REGISTERED;
  explicit platform(detail::platform_impl* impl);
};

bool operator==(const platform& lhs, const platform& rhs) {
  return lhs.impl_ == rhs.impl_;
}
bool operator!=(const platform& lhs, const platform& rhs) {
  return !(lhs == rhs);
}

} // namespace neosycl::sycl
