#ifndef SYCL_INCLUDE_CL_SYCL_NEC_VE_SELECTOR_HPP_
#define SYCL_INCLUDE_CL_SYCL_NEC_VE_SELECTOR_HPP_

#if 0
#include "neoSYCL/extensions/nec/ve_info.hpp"
#include "neoSYCL/extensions/nec/ve_kernel_info.hpp"
#include "neoSYCL/extensions/nec/ve_task_handler.hpp"
#include "neoSYCL/extensions/nec/ve_device_info.hpp"
#include "neoSYCL/sycl/detail/context_info.hpp"
#include "neoSYCL/extensions/nec/ve_context_info.hpp"
#endif
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

#ifdef BUILD_VE
platform platform::register_all_devices() {
  // create a platform with the default device at first
  initial_platform_builder builder;
  platform p(builder.create());
  // register all available devices
  builder.add<detail::ve_device_impl>(p);
  return p;
}
#endif

} // namespace neosycl::sycl

#endif // SYCL_INCLUDE_CL_SYCL_NEC_VE_SELECTOR_HPP_
