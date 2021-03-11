#ifndef CUSTOM_SYCL_INCLUDE_SYCL_PLATFORM_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_PLATFORM_HPP_

#include "device_selector.hpp"
#include "info/device_type.hpp"
#include "info/param_traits.hpp"
#include "detail/platform_info.hpp"
#include "detail/device_type.hpp"
#include "detail/registered_platforms.hpp"

namespace neosycl::sycl {

class device;

class platform {
public:
  platform() : platform_info(new detail::default_platform_info) {}

  platform(const shared_ptr_class<detail::platform_info> &info) : platform_info(info) {}

//  explicit platform(cl_platform_id platformID);

  explicit platform(const device_selector &deviceSelector) :
      platform_info(deviceSelector.get_platform_info()) {}

/* -- common interface members -- */
//  cl_platform_id get() const;

  vector_class<device> get_devices(info::device_type = info::device_type::all) const;

  template<info::platform param>
  typename info::param_traits<info::platform, param>::return_type get_info() const;

  bool has_extension(const string_class &extension) const {
    return platform_info->has_extension(extension);
  }

  bool is_host() const {
    return platform_info->is_host();
  }

  static vector_class<platform> get_platforms() {
    vector_class<platform> ret;
    for (const shared_ptr_class<detail::platform_info> &info: detail::REGISTERED_PLATFORMS) {
      ret.push_back(platform(info));
    }
    return ret;
  }

private:
  shared_ptr_class<detail::platform_info> platform_info;
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_PLATFORM_HPP_
