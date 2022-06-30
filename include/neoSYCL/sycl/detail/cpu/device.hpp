#pragma once

namespace neosycl::sycl::detail {

struct device_impl_cpu : public device_impl {
  device_impl_cpu(device d) : device_impl(d) {}

  bool is_host() override {
    return false;
  }
  bool is_cpu() override {
    return true;
  }
  bool is_gpu() override {
    return false;
  }
  bool is_accelerator() override {
    return false;
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
    return info::device_type::cpu;
  }

  virtual program_data* create_program(device d) override {
    return new program_data_cpu(d);
  }
};

} // namespace neosycl::sycl::detail
