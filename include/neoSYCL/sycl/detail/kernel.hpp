#pragma once
#include <utility>
#include <dlfcn.h>
#include <map>

namespace neosycl::sycl {

namespace detail {

struct kernel_data {
  kernel_data() {}
  virtual ~kernel_data() {}
};

struct kernel_data_host : public kernel_data {
  kernel_data_host() : kernel_data() {}
  virtual ~kernel_data_host() {}
};

class kernel_impl {
  // friend class program_impl;
  friend class kernel;

public:
  using accessor_list = kernel::accessor_list;

  kernel_impl(string_class n, program p) : name(n), prog(p), acc_() {}
  virtual ~kernel_impl() = default;

  string_class name;
  program prog;
#ifndef DISABLE_MULTI_DEVICE_SUPPORT
  using kernel_data_map =
      std::map<info::device_type, shared_ptr_class<kernel_data>>;
  kernel_data_map map;
#else
  shared_ptr_class<kernel_data> data;
#endif
  accessor_list acc_;
};

} // namespace detail

kernel::kernel() : impl_(nullptr) {}

kernel::kernel(string_class name, program prog) : impl_(nullptr) {
  impl_ = std::move(shared_ptr_class<detail::kernel_impl>(
      new detail::kernel_impl(name, prog)));
}

bool kernel::is_host() const {
  return get_program().is_host();
}

context kernel::get_context() const{
  return get_program().get_context();
}

program kernel::get_program() const{
  return impl_->prog;
}

shared_ptr_class<detail::kernel_data> kernel::get_kernel_data(device d) {
#ifndef DISABLE_MULTI_DEVICE_SUPPORT
  return impl_->map.at(d.type());
#else
  return impl_->data;
#endif
}

const char* kernel::get_name() const {
  return impl_->name.c_str();
}

void kernel::set_acc(accessor_list& acc) {
  impl_->acc_ = acc;
}

kernel::accessor_list& kernel::kernel::get_acc() {
  return impl_->acc_;
}

} // namespace neosycl::sycl
