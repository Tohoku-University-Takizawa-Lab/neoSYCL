#pragma once
#include "neoSYCL/sycl/access.hpp"
#include "neoSYCL/sycl/info/kernel.hpp"

namespace neosycl::sycl {

namespace detail {
class kernel_impl;
class kernel_data;
} // namespace detail

class program;
class handler;

///////////////////////////////////////////////////////////////////////////////
class kernel {
  friend class handler;

  explicit kernel();

public:
  kernel(string_class name, program prog);
  kernel(const kernel& k) : impl_(k.impl_) {}
  ~kernel() = default;

  /* -- common interface members -- */
  cl_kernel get() const {
    return 0;
  }

  bool is_host() const;

  context get_context() const;

  program get_program() const;

  template <info::kernel param>
  typename info::param_traits<info::kernel, param>::return_type
  get_info() const;

  template <info::kernel_work_group param>
  typename info::param_traits<info::kernel_work_group, param>::return_type
  get_work_group_info(const device& dev) const;

  // INTERNAL USE ONLY: for debugging
  const char* get_name() const;
  shared_ptr_class<detail::kernel_data> get_kernel_data(device);
  shared_ptr_class<detail::kernel_impl> get_impl() {
    return impl_;
  }

private:
  shared_ptr_class<detail::kernel_impl> impl_;
};

} // namespace neosycl::sycl
