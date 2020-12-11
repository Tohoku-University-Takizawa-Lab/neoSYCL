#ifndef CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_SELECTOR_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_SELECTOR_HPP_

#include "device.hpp"
#include "exception.hpp"

namespace neosycl::sycl {

class device_selector {
 public:

  virtual device select_device() const = 0;

  virtual int operator()(const device &dev) const = 0;

};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_SELECTOR_HPP_
