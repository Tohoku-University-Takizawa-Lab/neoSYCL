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
  friend class program_impl;
  friend class kernel;

public:
  using kernel_data_map =
      std::map<info::device_type, shared_ptr_class<kernel_data>>;

  kernel_impl(string_class n, program p) : name(n), prog(p) {}
  virtual ~kernel_impl() = default;

  string_class name;
  program prog;
  kernel_data_map map;
};

class kernel_data_cpu : public kernel_data {
public:
  void* dll_;
  int (*func_)();
  void* capt_;
  void* rnge_;

  kernel_data_cpu() : kernel_data() {
    dll_  = nullptr;
    func_ = nullptr;
    capt_ = nullptr;
    rnge_ = nullptr;
  }
};

} // namespace detail

kernel::kernel() : acc_(), impl_(nullptr) {}

kernel::kernel(string_class name, program prog) : acc_(), impl_(nullptr) {
  impl_ = std::move(shared_ptr_class<detail::kernel_impl>(
      new detail::kernel_impl(name, prog)));
}

shared_ptr_class<detail::kernel_data> kernel::get_kernel_data(device d) {
  return impl_->map.at(d.type());
}

const char* kernel::get_name() const {
  return impl_->name.c_str();
}
} // namespace neosycl::sycl