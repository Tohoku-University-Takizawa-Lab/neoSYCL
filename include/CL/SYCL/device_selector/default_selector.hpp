//
// Created by WhiteBlue on 2020/6/22.
//

#ifndef SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_DEFAULT_SELECTOR_HPP_
#define SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_DEFAULT_SELECTOR_HPP_

namespace cl::sycl {

class default_selector : public device_selector {

 public:

  int operator()(const device &dev) const override {
    return 1;
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_DEFAULT_SELECTOR_HPP_
