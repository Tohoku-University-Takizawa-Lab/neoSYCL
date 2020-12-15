#ifndef CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_SELECTOR_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_SELECTOR_HPP_

#include "device.hpp"

namespace neosycl::sycl {

class device;

class device_selector {
 public:

  device_selector();

  device_selector(const device_selector &rhs);

  device_selector &operator=(const device_selector &rhs);

  virtual ~device_selector();

  device select_device() const;

  virtual int operator()(const device &device) const = 0;

};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_DEVICE_SELECTOR_HPP_
