#pragma once

namespace neosycl::sycl::extensions::nec {

struct device_impl_ve : public detail::device_impl {
  device_impl_ve(device d) : detail::device_impl(d) {}

  bool is_host() override {
    return false;
  }
  bool is_cpu() override {
    return false;
  }
  bool is_gpu() override {
    return false;
  }
  bool is_accelerator() override {
    return true;
  }
  const void* get_info(info::device param) const override {
    switch (param) {
    case info::device::device_type:
    default:
      PRINT_ERR("device::get_info(%d) not implemented", (int)param);
      throw unimplemented();
    }
  }
  info::device_type type() override {
    return info::device_type::accelerator;
  }

  virtual detail::program_data* create_program(device d) override {
    return new program_data_ve(d);
  }
};
} // namespace neosycl::sycl::extensions::nec
