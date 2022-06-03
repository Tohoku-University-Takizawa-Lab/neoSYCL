#pragma once
#include <cstring>
//#include "neoSYCL/sycl/detail/program.hpp"

namespace neosycl::sycl {

namespace detail {

struct device_impl {
  using container_ptr = device::container_ptr;

  device dev_;
  device_impl(device d) : dev_(d) {}
  virtual ~device_impl() = default;

  virtual bool is_host()           = 0;
  virtual bool is_cpu()            = 0;
  virtual bool is_gpu()            = 0;
  virtual bool is_accelerator()    = 0;
  virtual info::device_type type() = 0;

  virtual void run(kernel k)                                                = 0;
  virtual void* get_pointer(container_ptr)                                  = 0;
  virtual void* alloc_mem(container_ptr, access::mode = access::mode::read) = 0;
  virtual void free_mem(container_ptr)                                      = 0;
  virtual void copy_back()                                                  = 0;
  virtual void set_capture(kernel&, void* p, size_t sz)                     = 0;
  virtual void set_range(kernel&, size_t r[6])                              = 0;
  virtual program_data* create_program(device)                              = 0;

  template <size_t dim>
  void set_range(kernel& k, range<dim> r) {
    size_t sz[6] = {1, 1, 1, 0, 0, 0};
    for (size_t idx(0); idx != dim; idx++) {
      sz[idx] = r[idx];
    }
    set_range(k, sz);
  }

  template <size_t dim>
  void set_range(kernel& k, range<dim> r, id<dim> i) {
    size_t sz[6] = {1, 1, 1, 0, 0, 0};
    for (size_t idx(0); idx != dim; idx++) {
      sz[idx] = r[idx];
    }
    for (size_t idx(3); idx != dim + 3; idx++) {
      sz[idx] = i[idx];
    }
    set_range(k, sz);
  }
};

struct device_impl_cpu : public device_impl {
  device_impl_cpu(device d) : device_impl(d) {}

  bool is_host() override { return false; }
  bool is_cpu() override { return true; }
  bool is_gpu() override { return false; }
  bool is_accelerator() override { return false; }
  info::device_type type() override { return info::device_type::cpu; }

  virtual void run(kernel k) override {
    auto kd  = k.get_kernel_data(dev_);
    auto kdc = std::dynamic_pointer_cast<kernel_data_cpu>(kd);
    if (kdc.get() == nullptr) {
      PRINT_ERR("invalid kernel_info: %lx", (size_t)kd.get());
      throw exception("run() failed");
    }
    for (const detail::accessor_info& acc : k.get_acc()) {
      acc.acquire_access();
      alloc_mem(acc.container, acc.mode);
    }
    DEBUG_INFO("-- KENREL EXEC BEGIN --");
    try {
#ifdef DEBUG
      int ret_val = kdc->func_();
#else
      kdc->func_();
#endif
      DEBUG_INFO("-- KERNEL EXEC END (ret=%d) --", ret_val);
    }
    catch (exception& e) {
      PRINT_ERR("kernel execution failed: %s", e.what());
      throw;
    }
    for (const detail::accessor_info& acc : k.get_acc()) {
      acc.release_access();
    }
  }

  void* get_pointer(container_ptr p) override { return p->get_raw_ptr(); }
  void* alloc_mem(container_ptr p, access::mode = access::mode::read) override {
    return nullptr;
  }
  void free_mem(container_ptr) override {}
  void copy_back() override {}

  void set_capture(kernel& k, void* p, size_t sz) override {
    auto ki =
        std::dynamic_pointer_cast<kernel_data_cpu>(k.get_kernel_data(dev_));
    if (ki && ki->capt_)
      std::memcpy(ki->capt_, p, sz);
  }

  void set_range(kernel& k, size_t r[6]) override {
    auto ki =
        std::dynamic_pointer_cast<kernel_data_cpu>(k.get_kernel_data(dev_));
    if (ki && ki->rnge_)
      std::memcpy(ki->rnge_, r, sizeof(size_t) * 6);
  }

  virtual program_data* create_program(device d) override {
    return new program_data_cpu(d);
  }
  // context_info* create_context_info(device) const override;
};

/* TODO: this should be host */
using default_device_impl = device_impl_cpu;

} // namespace detail

device device::get_default_device() {
  return platform::get_default_platform().get_devices()[0];
}

bool device::is_host() const { return impl_->is_host(); }

bool device::is_cpu() const { return impl_->is_cpu(); }

bool device::is_gpu() const { return impl_->is_gpu(); }

bool device::is_accelerator() const { return impl_->is_accelerator(); }

info::device_type device::type() const { return impl_->type(); }

detail::program_data* device::create_program() const {
  return impl_->create_program(*this);
}

} // namespace neosycl::sycl
