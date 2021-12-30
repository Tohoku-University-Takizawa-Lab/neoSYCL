#ifndef SYCL_INCLUDE_CL_SYCL_NEC_VE_DEVICE_INFO_HPP_
#define SYCL_INCLUDE_CL_SYCL_NEC_VE_DEVICE_INFO_HPP_

namespace neosycl::sycl {

struct ve_device_info : public detail::device_info {
  bool is_host() override { return false; }

  bool is_cpu() override { return true; }
  bool is_gpu() override { return false; }
  bool is_accelerator() override { return true; }

  detail::SUPPORT_PLATFORM_TYPE type() override {
    return detail::SUPPORT_PLATFORM_TYPE::VE;
  }

  detail::context_info *create_context_info() const override;
};

} // namespace neosycl::sycl

#endif // SYCL_INCLUDE_CL_SYCL_NEC_VE_DEVICE_INFO_HPP_
