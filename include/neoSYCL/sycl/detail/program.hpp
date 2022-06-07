#pragma once
#include <map>
#include <regex>
#include <cxxabi.h>
#include "neoSYCL/sycl/detail/container/data_container.hpp"
#include "neoSYCL/sycl/detail/container/data_container_nd.hpp"
#include "neoSYCL/sycl/detail/container/buffer_container.hpp"

namespace neosycl::sycl {

class handler;

namespace detail::container {
class CopybackProxy;
}

namespace detail {
const char* DEFAULT_LIB = "./kernel.so";
const char* ENV_KERNEL  = "NEOSYCL_KERNEL";

/* convet a type to its name in string */
inline string_class get_kernel_name_from_class(const std::type_info& ti) {
  int status;
  char* pc = abi::__cxa_demangle(ti.name(), 0, 0, &status);
  string_class in(pc);
  free(pc);
  std::regex re("([^\\s\\:]+)\\*$");
  std::smatch result;
  if (std::regex_search(in, result, re)) {
    in = result[1].str();
  }
  return in;
}

///////////////////////////////////////////////////////////////////////////////
// internal program state on each device and interface to handle the device
class program_data {
public:
  friend class neosycl::sycl::handler;
  friend class container::CopybackProxy;

  using kernel_data_ptr = shared_ptr_class<kernel_data>;
  // don't copy the instance
  program_data(const program_data& rhs) = delete;
  program_data(program_data&& rhs)      = delete;
  program_data& operator=(const program_data& rhs) = delete;
  program_data& operator=(program_data&& rhs) = delete;

  program_data(device d) : dev_(d) {}
  virtual ~program_data() {}

  device& get_device() {
    return dev_;
  }

  virtual bool open()                                       = 0;
  virtual bool is_open()                                    = 0;
  virtual void run(kernel k)                                = 0;
  virtual kernel_data_ptr create_kernel_data(const char* s) = 0;

protected:
  device dev_;

  virtual void* alloc_mem(void*, size_t, access::mode)  = 0;
  virtual void free_mem(void*)                          = 0;
  virtual void write_mem(void*, void*, size_t)          = 0;
  virtual void read_mem(void*, void*, size_t)           = 0;
  virtual void copy_mem(void*, void*, size_t)           = 0;
  virtual void set_capture(kernel&, void* p, size_t sz) = 0;
  virtual void set_range(kernel&, size_t r[6])          = 0;

  template <int dim>
  void set_range(kernel& k, range<dim> r) {
    size_t sz[6] = {1, 1, 1, 0, 0, 0};
    for (size_t idx(0); idx != dim; idx++) {
      sz[idx] = r[idx];
    }
    set_range(k, sz);
  }

  template <int dim>
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

  template <typename T>
  auto cast(kernel k) {
    auto kd  = k.get_kernel_data(get_device());
    auto kdc = std::dynamic_pointer_cast<T>(kd);
    if (kdc.get() == nullptr) {
      PRINT_ERR("invalid kernel_data: %lx", (size_t)kd.get());
      throw runtime_error("program_data::cast() failed");
    }
    return kdc;
  }
};

///////////////////////////////////////////////////////////////////////////////
// implementation of program class interface
class program_impl {
  friend class sycl::program;
  using kernel_data_ptr = program_data::kernel_data_ptr;
  using kernel_hash_map = std::map<size_t, kernel>;
  using kernel_name_map = std::map<string_class, kernel>;

  program_impl(program p, context c, vector_class<device> d)
      : prog_(p), ctx_(c) {
    init(d);
  }

  void init(const vector_class<device>& d) {
    if (d.size() == 0)
      throw runtime_error("contenxt with no available device");
    if (d.size() > 2) {
      DEBUG_INFO("one context with host and multiple devices (experimental)");
    }
    for (const auto& dev : d) {
      program_data* p = dev.create_program();
      if (p && p->open())
        data_.push_back(shared_ptr_class<program_data>(p));
      else if (p)
        delete p;
    }
    if (data_.size() == 0)
      throw runtime_error("program initialization failed");
  }

public:
  ~program_impl() = default;

  bool is_host() const {
    // true if only the host is available.
    return data_.size() < 2;
  }

  bool has_kernel(string_class name) {
    return name_map_.count(name);
  }

  template <typename KernelName>
  bool has_kernel() {
    const std::type_info& tinfo = typeid(KernelName*);
    if (hash_map_.count(tinfo.hash_code()))
      return true;

    string_class name = get_kernel_name_from_class(tinfo);
    return has_kernel(name);
  }

  kernel get_kernel(string_class name) {
    if (name_map_.count(name)) {
      DEBUG_INFO("kernel found: %s", name.c_str());
      return name_map_.at(name);
    }

    kernel k(name.c_str(), prog_);
#ifndef DISABLE_MULTI_DEVICE_SUPPORT
    for (auto& d : data_) {
      auto kd = d->create_kernel_data(name.c_str());
      k.get_impl()->map.insert(std::make_pair(d->get_device().type(), kd));
    }
#else
    auto kd            = data_[0]->create_kernel_data(name.c_str());
    k.get_impl()->data = std::move(kd);
#endif
    name_map_.insert(std::make_pair(name, k));
    return k;
  }

  template <typename KernelName>
  kernel get_kernel() {
    const std::type_info& tinfo = typeid(KernelName*);
    if (hash_map_.count(tinfo.hash_code())) {
      DEBUG_INFO("kernel found: hash=%lu", tinfo.hash_code());
      return hash_map_.at(tinfo.hash_code());
    }

    string_class name = get_kernel_name_from_class(tinfo);
    DEBUG_INFO("kernel class: %s", name.c_str());

    kernel k = std::move(get_kernel(name));
    hash_map_.insert(std::make_pair(tinfo.hash_code(), k));
    return k;
  }

  context get_context() const {
    return ctx_;
  }

private:
  program_state state_;
  program prog_;
  context ctx_;
  vector_class<shared_ptr_class<program_data>> data_;
  kernel_hash_map hash_map_; // quickly accessible
  kernel_name_map name_map_; // all exiting kernels
};

class program_data_host : public program_data {
public:
  program_data_host(device d) : program_data(d) {}

  ~program_data_host() {}

  bool open() override {
    return true;
  }

  bool is_open() override {
    return true;
  }

  void run(kernel k) override {}

  void* alloc_mem(void* p, size_t s, access::mode m) override {
    return nullptr;
  }

  void free_mem(void* p) override {}

  void write_mem(void* d, void* h, size_t s) override {}

  void read_mem(void* h, void* d, size_t s) override {}

  void copy_mem(void* h, void* d, size_t s) override {}

  void set_capture(kernel& k, void* p, size_t sz) override {}

  void set_range(kernel& k, size_t r[6]) override {}

  kernel_data_ptr create_kernel_data(const char* s) override {
    auto data = new kernel_data_host();
    kernel_data_ptr ret(data);
    return ret;
  }
};
} // namespace detail

void program::init_(context c, vector_class<device> d) {
  impl_ = shared_ptr_class<detail::program_impl>(
      new detail::program_impl(*this, c, d));
}

bool program::is_host() const {
  return impl_->is_host();
}

template <typename kernelT>
bool program::has_kernel() const {
  return impl_->has_kernel<kernelT>();
}

bool program::has_kernel(string_class kernelName) const {
  return impl_->has_kernel(kernelName);
}

kernel program::get_kernel(string_class kernelName) const {
  return impl_->get_kernel(kernelName);
}

template <typename kernelT>
kernel program::get_kernel() const {
  return impl_->get_kernel<kernelT>();
}

context program::get_context() const {
  return impl_->get_context();
}

vector_class<device> program::get_devices() const {
  vector_class<device> ret;
  for (const auto& dat : impl_->data_)
    ret.push_back(dat->get_device());
  return ret;
}

program_state program::get_state() const {
  return impl_->state_;
}

shared_ptr_class<detail::program_data> program::get_data(device dev) const {
  for (auto& d : impl_->data_)
    if (dev == d->get_device())
      return d;
  return nullptr;
}

namespace detail::container {

///////////////////////////////////////////////////////////////////////////////
// Copyback device data to host memory at buffer object destruction
class CopybackProxy {
  // buffer must be copied back even after a queue/program is destoryed.
  // so shared_ptr<program_data> is kept in the map to do it.
public:
  template <typename T, int D, typename A = buffer_allocator<T>>
  void operator()(BufferContainer<T, D, A>& buf,
                  shared_ptr_class<program_data> p, bool free_mem_flag) {
    DEBUG_INFO("memory copy back : device type = %d",
               (int)p->get_device().type());

    if (p->get_device().is_host())
      return;
    if (buf.map.count(p)) {
      auto [devp, mode] = buf.map.at(p);
      if (mode != access::mode::read)
        p->read_mem(buf.get_raw_ptr(), devp, buf.get_size());
      // buf.map.erase(p);
      if (free_mem_flag)
        p->free_mem(devp);
    }
  }
};

template <typename T, int D, typename A>
BufferContainer<T, D, A>::~BufferContainer() {
  CopybackProxy proxy;
  DEBUG_INFO("buffer destruction: %lu device(s)", map.size());
  for (auto& d : map) {
    proxy(*this, d.first, true);
  }
}
} // namespace detail::container

} // namespace neosycl::sycl
