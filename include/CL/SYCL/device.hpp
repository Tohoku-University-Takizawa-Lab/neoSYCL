#ifndef CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_

#include "CL/SYCL/exception.hpp"
#include "CL/SYCL/types.hpp"
#include "CL/SYCL/info/device_type.hpp"

namespace cl::sycl {

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

  static vector_class<device> get_devices(info::device_type deviceType = info::device_type::all);
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_HPP_
