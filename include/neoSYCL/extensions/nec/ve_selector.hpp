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
  int operator()(const device &dev) const override {
    if (dev.is_accelerator()) {
      return true;
    }
    return false;
  }
  device select_device() const override {
    return device(shared_ptr_class<detail::device_info>(new ve_device_info()));
  }
};

detail::context_info *ve_device_info::create_context_info() const {
  return new extensions::nec::ve_context_info();
}

} // namespace neosycl::sycl

#endif // SYCL_INCLUDE_CL_SYCL_NEC_VE_SELECTOR_HPP_
