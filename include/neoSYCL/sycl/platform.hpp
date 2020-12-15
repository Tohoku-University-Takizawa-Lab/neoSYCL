#ifndef CUSTOM_SYCL_INCLUDE_SYCL_PLATFORM_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_PLATFORM_HPP_

#include "device_selector.hpp"
#include "info/device_type.hpp"
#include "info/param_traits.hpp"

namespace neosycl::sycl {

class device;

class platform {
 public:

  platform();

//  explicit platform(cl_platform_id platformID);

  explicit platform(const device_selector &deviceSelector);

/* -- common interface members -- */
//  cl_platform_id get() const;

  vector_class<device> get_devices(info::device_type = info::device_type::all) const;

  template<info::platform param>
  typename info::param_traits<info::platform, param>::return_type get_info() const;

  bool has_extension(const string_class &extension) const;

  bool is_host() const;

  static vector_class<platform> get_platforms();

};

platform::platform(const device_selector &deviceSelector) {

}

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_PLATFORM_HPP_
