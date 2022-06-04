#pragma once
#include <cstring>
//#include "neoSYCL/sycl/detail/program.hpp"

namespace neosycl::sycl {

namespace detail {

struct device_impl {
  device dev_;
  device_impl(device d) : dev_(d) {}
  virtual ~device_impl() = default;

  virtual bool is_host()           = 0;
  virtual bool is_cpu()            = 0;
  virtual bool is_gpu()            = 0;
  virtual bool is_accelerator()    = 0;
  virtual info::device_type type() = 0;

  template <typename T, size_t D, typename A = buffer_allocator<T>>
  void* get_pointer(container::BufferContainer<T, D, A>& buf) {
    if (is_host())
      return buf.get_raw_ptr();
    else if (buf.map.count(dev_))
      return buf.map.at(dev_).ptr;
    throw runtime_error("invalid BufferContainer object");
  }

  template <typename T, size_t D, typename A = buffer_allocator<T>>
  void* alloc_mem(container::BufferContainer<T, D, A>& buf,
                  access::mode m = access::mode::read) {
    if (is_host())
      return buf.get_raw_ptr();

    int count = buf.map.count(dev_);
    if (count == 1)
      return buf.map.at(dev_).ptr;
    else if (count == 0) {
      void* dp = alloc_mem(buf.get_raw_ptr(), buf.get_size(), m);
      container::device_ptr cdp = {dp, m};
      buf.map.insert(std::make_pair(dev_, cdp));
      return dp;
    }
    throw runtime_error("invalid BufferContainer object");
  }

  template <typename T, size_t D, typename A = buffer_allocator<T>>
  void free_mem(container::BufferContainer<T, D, A>& buf) {
    if (is_host() == false && buf.map.count(dev_)) {
      auto [devp, mode] = buf.map.at(dev_);
      if (mode != access::mode::read)
        copy_back(buf.get_raw_ptr(), devp, buf.get_size());
      // buf.map.erase(dev_);
      free_mem(devp);
    }
  }

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

  virtual void run(kernel k)                            = 0;
  virtual void set_capture(kernel&, void* p, size_t sz) = 0;
  virtual void set_range(kernel&, size_t r[6])          = 0;
  virtual program_data* create_program(device)          = 0;

protected:
  virtual void* alloc_mem(void*, size_t, access::mode) = 0;
  virtual void free_mem(void*)                         = 0;
  virtual void copy_back(void*, void*, size_t)         = 0;
};

struct device_impl_host : public device_impl {
  device_impl_host(device d) : device_impl(d) {}

  bool is_host() override {
    return true;
  }
  bool is_cpu() override {
    return false;
  }
  bool is_gpu() override {
    return false;
  }
  bool is_accelerator() override {
    return false;
  }
  info::device_type type() override {
    return info::device_type::host;
  }

  void run(kernel k) override {}

  void* alloc_mem(void* p, size_t s, access::mode m) override {
    return nullptr;
  }

  void free_mem(void* p) override {}

  void copy_back(void* h, void* d, size_t s) override {}

  void set_capture(kernel& k, void* p, size_t sz) override {}

  void set_range(kernel& k, size_t r[6]) override {}

  virtual program_data* create_program(device d) override {
    return new program_data_host(d);
  }
  // context_info* create_context_info(device) const override;
};

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
  info::device_type type() override {
    return info::device_type::cpu;
  }

  void run(kernel k) override {
    auto kd  = k.get_kernel_data(dev_);
    auto kdc = std::dynamic_pointer_cast<kernel_data_cpu>(kd);
    if (kdc.get() == nullptr) {
      PRINT_ERR("invalid kernel_info: %lx", (size_t)kd.get());
      throw exception("run() failed");
    }
    for (const auto& acc : k.get_acc()) {
      acc.acquire_access();
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
    for (const auto& acc : k.get_acc()) {
      acc.release_access();
    }
  }

  void* alloc_mem(void* p, size_t s, access::mode m) override {
    void* ret = malloc(s);
    if (m != access::mode::discard_write &&
        m != access::mode::discard_read_write)
      std::memcpy(ret, p, s);
    return ret;
  }

  void free_mem(void* p) override {
    if (p)
      free(p);
  }

  void copy_back(void* h, void* d, size_t s) override {
    std::memcpy(h, d, s);
  }

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
};

using default_device_impl = device_impl_host;

} // namespace detail

device device::get_default_device() {
#ifdef USE_CPU
  return platform::get_default_platform().get_devices()[1];
#else
  return platform::get_default_platform().get_devices()[0];
#endif
}

bool device::is_host() const {
  return impl_->is_host();
}

bool device::is_cpu() const {
  return impl_->is_cpu();
}

bool device::is_gpu() const {
  return impl_->is_gpu();
}

bool device::is_accelerator() const {
  return impl_->is_accelerator();
}

info::device_type device::type() const {
  return impl_->type();
}

detail::program_data* device::create_program() const {
  return impl_->create_program(*this);
}

namespace detail::container {
template <typename T, size_t D, typename A>
BufferContainer<T, D, A>::~BufferContainer() {
  for (auto& d : map) {
    d.first.get_impl()->free_mem(*this);
  }
}
} // namespace detail::container
} // namespace neosycl::sycl
