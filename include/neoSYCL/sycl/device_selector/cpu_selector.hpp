#ifndef SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_CPU_SELECTOR_HPP_
#define SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_CPU_SELECTOR_HPP_

namespace neosycl::sycl {

class cpu_selector : public device_selector {

public:
  int operator()(const device &dev) const override {
    if (dev.is_cpu()) {
      return true;
    }
    return false;
  }

  device select_device() const {
    return device(
        shared_ptr_class<detail::device_info>(new detail::cpu_device_info()));
  }
};

using default_selector = cpu_selector;
using host_selector    = cpu_selector;

} // namespace neosycl::sycl

#endif // SYCL_INCLUDE_CL_SYCL_DEVICE_SELECTOR_CPU_SELECTOR_HPP_
