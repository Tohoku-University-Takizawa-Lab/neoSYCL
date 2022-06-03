#pragma once
#include <map>
#include <regex>
#include <cxxabi.h>

namespace neosycl::sycl {

namespace detail {
const char* DEFAULT_LIB = "./kernel.so";
const char* ENV_KERNEL  = "NEOSYCL_KERNEL";

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

class program_data {
public:
  program_data(device d) : dev(d) {}
  virtual ~program_data() {}
  // using kernel_data_map = std::map<string_class, kernel_data*>;
  using kernel_data_ptr = shared_ptr_class<kernel_data>;

  virtual bool open()                                       = 0;
  virtual bool is_valid()                                   = 0;
  virtual kernel_data_ptr create_kernel_data(const char* s) = 0;

  // kernel_data_map map;
  device dev;
};

class program_impl {
  friend class sycl::program;
  using kernel_data_ptr = program_data::kernel_data_ptr;
  using kernel_map      = std::map<string_class, kernel>;

  program_impl(program p, context c, vector_class<device> d)
      : prog_(p), ctx_(c) {
    init(d);
  }

  void init(const vector_class<device>& d) {
    for (auto& dev : d) {
      program_data* p = dev.create_program();
      if (p && p->open())
        data_.push_back(shared_ptr_class<program_data>(p));
      else
        delete p;
    }
  }

public:
  ~program_impl() = default;

  /* host program not supported yet */
  bool is_host() const { return false; }

  bool has_kernel(string_class name) { return kernels_.count(name); }

  template <typename KernelName>
  bool has_kernel() {
    const std::type_info& tinfo = typeid(KernelName*);

    string_class name = get_kernel_name_from_class(tinfo);
    return has_kernel(name);
  }

  kernel get_kernel(string_class name) {
    if (kernels_.count(name)) {
      DEBUG_INFO("kernel found: %s", name.c_str());
      return kernels_.at(name);
    }

    kernel k(name.c_str(), prog_);
    for (auto& d : data_) {
      auto kd = d->create_kernel_data(name.c_str());
      k.get_impl()->map.insert(std::make_pair(d->dev.type(), kd));
    }
    // kernel k = task_handler->create_kernel(name.c_str());
    kernels_.insert(std::make_pair(name, k));
    return k;
  }

  template <typename KernelName>
  kernel get_kernel() {
    const std::type_info& tinfo = typeid(KernelName*);

    string_class name = get_kernel_name_from_class(tinfo);
    DEBUG_INFO("kernel class: %s", name.c_str());
    return get_kernel(name);
  }

  context get_context() const { return ctx_; }

private:
  program prog_;
  context ctx_;
  program_state state;
  vector_class<shared_ptr_class<program_data>> data_;
  kernel_map kernels_; // all exiting kernels in the context
};

class program_data_cpu : public program_data {
  void* dll_;

public:
  program_data_cpu(device d) : program_data(d), dll_(nullptr) {}

  ~program_data_cpu() {
    if (dll_)
      dlclose(dll_);
  }

  bool open() override {
    const char* env = getenv(ENV_KERNEL);
    string_class fn(env ? env : DEFAULT_LIB);
    dll_ = dlopen(fn.c_str(), RTLD_LAZY);
    if (!dll_) {
      DEBUG_INFO("dlopen failed: %s", dlerror());
      return false;
    }
    DEBUG_INFO("kernel lib loaded: %lx, %s", (size_t)dll_, fn.c_str());
    return true;
  }

  bool is_valid() override { return dll_ != nullptr; }

  kernel_data_ptr create_kernel_data(const char* s) override {
    auto data         = new kernel_data_cpu();
    data->dll_        = dll_;
    string_class capt = string_class("__") + s + "_obj__";
    string_class rnge = string_class("__") + s + "_range__";
    auto f            = dlsym(dll_, s);
    data->func_       = reinterpret_cast<int (*)()>(f);
    data->capt_       = dlsym(dll_, capt.c_str());

    if (!data->dll_ || !data->func_ || !data->capt_) {
      PRINT_ERR("dlsym() for %s failed: %s", s, dlerror());
      throw exception("create_kernel() failed");
    }

    data->rnge_ = dlsym(dll_, rnge.c_str()); // this call could fail
    dlerror();                               // reset dlerror

    kernel_data_ptr ret(data);
    return ret;
  }
};
} // namespace detail

void program::init_(context c, vector_class<device> d) {
  impl_ = shared_ptr_class<detail::program_impl>(
      new detail::program_impl(*this, c, d));
}

template <typename kernelT>
bool program::has_kernel() const {
  return impl_->has_kernel<kernelT>();
}

bool program::has_kernel(string_class kernelName) const {
  return impl_->has_kernel(kernelName);
}

template <typename kernelT>
kernel program::get_kernel() const {
  return impl_->get_kernel<kernelT>();
}

kernel program::get_kernel(string_class kernelName) const {
  return impl_->get_kernel(kernelName);
}

} // namespace neosycl::sycl
