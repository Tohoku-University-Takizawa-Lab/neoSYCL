#pragma once

namespace neosycl::sycl::detail {

struct ve_device_info : public device_info {
  bool is_host() override { return false; }

  bool is_cpu() override { return true; }
  bool is_gpu() override { return false; }
  bool is_accelerator() override { return true; }

  info::device_type type() override { return info::device_type::accelerator; }

  context_info* create_context_info(device d) const override;
};

} // namespace neosycl::sycl::detail
