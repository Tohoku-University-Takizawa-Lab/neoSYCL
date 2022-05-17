#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_KERNEL_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_KERNEL_HPP

#include "neoSYCL/sycl/detail/kernel_info.hpp"
#include "neoSYCL/sycl/detail/accessor_info.hpp"

namespace neosycl::sycl {

class kernel {
public:
  using info_type     = shared_ptr_class<detail::kernel_info>;
  using accessor_list = vector_class<detail::accessor_info>;

  kernel(detail::kernel_info* info) : acc_(), info_(std::move(info)) {}

  void set_acc(accessor_list& acc) { acc_ = acc; }
  accessor_list& get_acc() { return acc_; }
  info_type get_kernel_info() { return info_; }
  // string_class get_name() { return info_->name; }
  const char* get_name() const { return info_->name.c_str(); }

private:
  info_type info_;
  accessor_list acc_;
};

} // namespace neosycl::sycl
#endif
