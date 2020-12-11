#ifndef SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_CPU_SELECTOR_HPP_
#define SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_CPU_SELECTOR_HPP_

namespace neosycl::sycl {

class cpu_selector : public device_selector {

 public:

  int operator()(const device &dev) const override {
    if (dev.is_cpu()) {
      return 1;
    }
    return -1;
  }

  device select_device() const override {
    return device();
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_CPU_SELECTOR_HPP_
