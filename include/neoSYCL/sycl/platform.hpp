#ifndef CUSTOM_SYCL_INCLUDE_SYCL_PLATFORM_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_PLATFORM_HPP_

#include "device.hpp"
#include "info/device_type.hpp"
#include "info/param_traits.hpp"

namespace neosycl::sycl {

class platform {
 public:

  platform() {

  }

  vector_class<device> get_devices(info::device_type device_type = info::device_type::all) const {
    throw UnimplementedException();
  }

  static vector_class<platform> get_platforms();

  template<info::platform param>
  typename info::param_traits<info::platform, param>::return_type get_info() const {
    throw UnimplementedException();
  }

  bool has_extension(const string_class &extension) const {
    throw UnimplementedException();
  }

  virtual bool is_host() const {
    return true;
  };

};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_PLATFORM_HPP_
