#pragma once
#include "neoSYCL/extensions/nec/veo_util.hpp"
#include "neoSYCL/extensions/nec/kernel.hpp"
#include "neoSYCL/extensions/nec/program.hpp"
#include "neoSYCL/extensions/nec/device.hpp"

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

platform platform::register_all_devices() {
  // create a platform with the default device at first
  initial_platform_builder builder;
  platform p(builder.create());
  // register all available devices
  builder.add<detail::device_impl_cpu>(p);
  builder.add<extensions::nec::device_impl_ve>(p);
  return p;
}

} // namespace neosycl::sycl
