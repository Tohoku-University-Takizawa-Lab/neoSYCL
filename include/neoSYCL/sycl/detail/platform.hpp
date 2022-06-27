#pragma once

#include "neoSYCL/sycl/detail/cpu/kernel.hpp"
#include "neoSYCL/sycl/detail/cpu/program.hpp"
#include "neoSYCL/sycl/detail/cpu/device.hpp"

namespace neosycl::sycl {

namespace detail {
class platform_impl {
public:
  platform_impl(device d) {
    dev_.push_back(d);
  }

  virtual bool is_host() = 0;

  virtual bool has_extension(const string_class& extension) = 0;

  vector_class<device> list_devices() {
    return dev_;
  }

  vector_class<device> dev_;
};

class host_platform_impl : public platform_impl {
public:
  host_platform_impl(device d) : platform_impl(d) {}

  bool is_host() override {
    return true;
  }
  bool has_extension(const string_class& extension) override {
    return false;
  }
};

using default_platform_impl = host_platform_impl;

} // namespace detail

vector_class<platform> platform::REGISTERED = {
    platform::register_all_devices()};

platform platform::get_default_platform() {
  return platform::REGISTERED[0];
}

vector_class<platform> platform::get_platforms() {
  return platform::REGISTERED;
}

vector_class<device> platform::get_devices(info::device_type t) const {
  vector_class<device> ret;
  for (const device& dev : impl_->list_devices()) {
    if (t == info::device_type::all || t == dev.type())
      ret.push_back(dev);
  }
  return ret;
}

platform::platform(const device_selector& deviceSelector) {
  device d = deviceSelector.select_device();
  impl_    = shared_ptr_class<detail::platform_impl>(
      new detail::host_platform_impl(d));
  for (auto& dev : impl_->dev_)
    dev.set_platform(*this);
}

platform::platform(detail::platform_impl* impl) : impl_(impl) {
  /* do nothing if impl == null */
  if (impl) {
    for (auto& dev : impl_->dev_)
      dev.set_platform(*this);
  }
}

bool platform::has_extension(const string_class& extension) const {
  return impl_->has_extension(extension);
}

bool platform::is_host() const {
  return impl_->is_host();
}

/* this class accesses private members of platform and device */
class initial_platform_builder {
public:
  template <typename T>
  device get() {
    device dummy(nullptr);
    device d(new T(dummy));
    d.get_impl()->dev_ = d;
    return d;
  }

  template <typename T>
  void add(platform& p) {
    device d = get<T>();
    d.set_platform(p);
    p.impl_->dev_.push_back(d);
  }

  platform create() {
    device d = get<detail::default_device_impl>();
    platform p(new detail::default_platform_impl(d));
    d.set_platform(p);
    return p;
  }
};

#ifndef BUILD_VE
// See also extensions/nec/ve_selector.hpp
platform platform::register_all_devices() {
  // create a platform with the default device at first
  initial_platform_builder builder;
  platform p(builder.create());
  // register all available devices
#ifdef USE_CPU
  builder.add<detail::device_impl_cpu>(p);
#else
  builder.add<detail::device_impl_host>(p);
#endif
  return p;
}
#endif

} // namespace neosycl::sycl
