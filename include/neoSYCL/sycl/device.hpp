#ifndef CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_

#include "exception.hpp"
#include "types.hpp"
#include "info/device_type.hpp"
#include "info/device.hpp"
#include "info/param_traits.hpp"

namespace neosycl::sycl {

class device_selector;

class device {
 public:

  virtual bool is_host() const {
    return true;
  };

  virtual bool is_cpu() const {
    return true;
  };

  virtual bool is_gpu() const {
    return false;
  };

  virtual bool is_accelerator() const {
    return false;
  };

  template<info::device param>
  typename info::param_traits<info::device, param>::return_type get_info() const {
    return "";
  }

  static vector_class<device> get_devices(info::device_type deviceType = info::device_type::all);

};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_
