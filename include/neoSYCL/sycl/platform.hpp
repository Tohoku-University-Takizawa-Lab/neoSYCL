#pragma once

#include "neoSYCL/sycl/info/device_type.hpp"
#include "neoSYCL/sycl/info/param_traits.hpp"
#include "neoSYCL/sycl/detail/platform_info.hpp"
#include "neoSYCL/sycl/detail/device_type.hpp"
#include "neoSYCL/sycl/detail/task_handler.hpp"

namespace neosycl::sycl {

class device;

class platform {
public:
  platform(const platform& rhs) = default;
  platform(platform&& rhs)      = default;
  ~platform()                   = default;

  platform& operator=(const platform& rhs) = default;
  platform& operator=(platform&& rhs) = default;

  friend bool operator==(const platform& lhs, const platform& rhs);
  friend bool operator!=(const platform& lhs, const platform& rhs);

  // get a static platform object by default (= REGISTERED[0])
  explicit platform() { *this = get_default_platform(); }

  explicit platform(cl_platform_id platformID) { throw unimplemented(); }

  explicit platform(
      const device_selector& deviceSelector) /* defined in device.hpp*/;

  // INTERNAL USE ONLY: create the default platform
  explicit platform(detail::platform_info* info) /* defined in device.hpp*/;

  /* -- common interface members -- */
  /* platform is not associated with OpenCL => 0  */
  cl_platform_id get() const { return 0; }

  vector_class<device>
      get_devices(info::device_type = info::device_type::all) const
      /* defined in device.hpp */;

  template <info::platform param>
  typename info::param_traits<info::platform, param>::return_type
  get_info() const {
    throw unimplemented();
  }

  bool has_extension(const string_class& extension) const {
    return info_->has_extension(extension);
  }

  bool is_host() const { return info_->is_host(); }

  static vector_class<platform> get_platforms() /* defined below */;
  static platform get_default_platform() /* defined below */;
  static platform register_all_devices() /* defined in device.hpp */;

private:
  shared_ptr_class<detail::platform_info> info_;

  static vector_class<platform> REGISTERED;
};

vector_class<platform> platform::REGISTERED = {
    platform::register_all_devices()};

platform platform::get_default_platform() { return platform::REGISTERED[0]; }

vector_class<platform> platform::get_platforms() {
  return platform::REGISTERED;
}

} // namespace neosycl::sycl
