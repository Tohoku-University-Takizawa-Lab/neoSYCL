#ifndef SYCL_INCLUDE_CL_SYCL_NEC_VE_SELECTOR_HPP_
#define SYCL_INCLUDE_CL_SYCL_NEC_VE_SELECTOR_HPP_

namespace neosycl::sycl {

class ve_selector : public device_selector {

public:
  int operator()(const device &dev) const override {
    if (dev.is_accelerator()) {
      return 1;
    }
    return -1;
  }

  device select_device() const override { return ve_device(); }
};

} // namespace neosycl::sycl

#endif // SYCL_INCLUDE_CL_SYCL_NEC_VE_SELECTOR_HPP_
