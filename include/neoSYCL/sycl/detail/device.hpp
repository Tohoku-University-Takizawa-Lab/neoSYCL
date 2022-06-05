#pragma once
#include <cstring>

namespace neosycl::sycl {

namespace detail {

struct device_impl {
  device dev_;
  device_impl(device d) : dev_(d) {}
  virtual ~device_impl() = default;

  virtual bool is_host()                       = 0;
  virtual bool is_cpu()                        = 0;
  virtual bool is_gpu()                        = 0;
  virtual bool is_accelerator()                = 0;
  virtual info::device_type type()             = 0;
  virtual program_data* create_program(device) = 0;
};

struct device_impl_host : public device_impl {
  device_impl_host(device d) : device_impl(d) {}

  bool is_host() override {
    return true;
  }
  bool is_cpu() override {
    return false;
  }
  bool is_gpu() override {
    return false;
  }
  bool is_accelerator() override {
    return false;
  }
  info::device_type type() override {
    return info::device_type::host;
  }

  virtual program_data* create_program(device d) override {
    return new program_data_host(d);
  }
};

using default_device_impl = device_impl_host;

} // namespace detail

device device::get_default_device() {
#if defined(USE_VE) && defined(BUILD_VE)
  return platform::get_default_platform().get_devices()[2];
#else
#if defined(USE_CPU)
  return platform::get_default_platform().get_devices()[1];
#else
  return platform::get_default_platform().get_devices()[0];
#endif
#endif
}

bool device::is_host() const {
  return impl_->is_host();
}

bool device::is_cpu() const {
  return impl_->is_cpu();
}

bool device::is_gpu() const {
  return impl_->is_gpu();
}

bool device::is_accelerator() const {
  return impl_->is_accelerator();
}

info::device_type device::type() const {
  return impl_->type();
}

detail::program_data* device::create_program() const {
  return impl_->create_program(*this);
}

} // namespace neosycl::sycl
