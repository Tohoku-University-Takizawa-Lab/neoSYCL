#pragma once

#include "neoSYCL/sycl/detail/context_info.hpp"

namespace neosycl::sycl {

class cpu_selector : public device_selector {

public:
  virtual int operator()(const device& dev) const override {
    if (dev.is_cpu()) {
      return 1;
    }
    return 0;
  }

  device select_device() const override {
    auto pf      = platform::get_default_platform();
    auto devices = pf.get_devices(info::device_type::cpu);
    for (auto& i : devices) {
      if (this->operator()(i) > 0)
        return i;
    }
    throw sycl::runtime_error("no available device found");
    // return device(new detail::cpu_device_info());
  }
};

using default_selector = cpu_selector;
using host_selector    = cpu_selector;

namespace detail {
context_info* cpu_device_info::create_context_info(device d) const {
  return new cpu_context_info(d);
}
} // namespace detail

} // namespace neosycl::sycl
