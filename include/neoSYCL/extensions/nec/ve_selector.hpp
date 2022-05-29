#ifndef SYCL_INCLUDE_CL_SYCL_NEC_VE_SELECTOR_HPP_
#define SYCL_INCLUDE_CL_SYCL_NEC_VE_SELECTOR_HPP_

#include "neoSYCL/extensions/nec/ve_info.hpp"
#include "neoSYCL/extensions/nec/ve_kernel_info.hpp"
#include "neoSYCL/extensions/nec/ve_task_handler.hpp"
#include "neoSYCL/extensions/nec/ve_device_info.hpp"
#include "neoSYCL/sycl/detail/context_info.hpp"
#include "neoSYCL/extensions/nec/ve_context_info.hpp"

namespace neosycl::sycl {

class ve_selector : public device_selector {

public:
  virtual int operator()(const device& dev) const override {
    if (dev.is_accelerator()) {
      return 1;
    }
    return 0;
  }
  device select_device() const override {
    auto pf      = platform::get_default_platform();
    auto devices = pf.get_devices(info::device_type::accelerator);
    for (auto& i : devices) {
      if (this->operator()(i) > 0)
        return i;
    }
    throw sycl::runtime_error("no available device found");
  }
};

detail::context_info*
detail::ve_device_info::create_context_info(device d) const {
  return new extensions::nec::ve_context_info(d);
}

#ifdef BUILD_VE
platform platform::register_all_devices() {
  device d(new detail::default_device_info());
  // create a platform with the default device at first
  platform p(new detail::default_platform_info(d));

  // register all available devices
  device ve(new detail::ve_device_info(), &p);
  p.info_->dev_.push_back(ve);
  return p;
}
#endif

} // namespace neosycl::sycl

#endif // SYCL_INCLUDE_CL_SYCL_NEC_VE_SELECTOR_HPP_
