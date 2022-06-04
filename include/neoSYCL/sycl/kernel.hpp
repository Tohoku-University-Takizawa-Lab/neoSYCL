#pragma once
#include "neoSYCL/sycl/access.hpp"
#include "neoSYCL/sycl/detail/accessor_data.hpp"

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
  using accessor_list = vector_class<detail::accessor_data>;

  kernel(string_class name, program prog);
  kernel(const kernel& k) : acc_(k.acc_), impl_(k.impl_) {}
  ~kernel() = default;

  // INTERNAL USE ONLY: for debugging
  const char* get_name() const;
  shared_ptr_class<detail::kernel_data> get_kernel_data(device);
  shared_ptr_class<detail::kernel_impl> get_impl() {
    return impl_;
  }
  void set_acc(accessor_list& acc) {
    acc_ = acc;
  }
  accessor_list& get_acc() {
    return acc_;
  }

private:
  accessor_list acc_;
  shared_ptr_class<detail::kernel_impl> impl_;
};

} // namespace neosycl::sycl
