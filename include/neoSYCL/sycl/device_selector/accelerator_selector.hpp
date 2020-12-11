#ifndef SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_ACCELERATOR_HPP_
#define SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_ACCELERATOR_HPP_

namespace neosycl::sycl {

class accelerator_selector : public device_selector {

 public:
  int operator()(const device &dev) const override {
    if (dev.is_accelerator()) {
      return 1;
    }
    return -1;
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_ACCELERATOR_HPP_
