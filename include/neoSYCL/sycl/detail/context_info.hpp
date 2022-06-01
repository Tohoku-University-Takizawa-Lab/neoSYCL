#pragma once
#include <map>
#include <regex>
#include <cxxabi.h>
#include "neoSYCL/sycl/detail/task_handler.hpp"

namespace neosycl::sycl::detail {
const char* DEFAULT_LIB = "./kernel.so";
const char* ENV_KERNEL  = "NEOSYCL_KERNEL";

inline string_class get_kernel_name_from_class(const std::type_info& ti) {
  // KernelName* p;
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

class context_info {
protected:
  context_info(device d) : bound_device(d), task_handler(nullptr) {}
  using container_ptr = detail::accessor_info::container_ptr;

public:
  using task_handler_ptr = shared_ptr_class<detail::task_handler>;
  using kernel_map       = std::map<size_t, kernel>;

  virtual ~context_info() = default;
  void* get_pointer(container_ptr c) const {
    return task_handler->get_pointer(c);
  }
  void* alloc_mem(container_ptr c, access::mode m = access::mode::read) const {
    return task_handler->alloc_mem(c, m);
  }
  void free_mem(container_ptr c) const { task_handler->free_mem(c); }

  void set_capture(kernel k, void* p, size_t sz) const {
    task_handler->set_capture(k, p, sz);
  }

  template <typename KernelName> kernel get_kernel() {
    const std::type_info& tinfo = typeid(KernelName*);

    if (kernels_.count(tinfo.hash_code())) {
      DEBUG_INFO("kernel found: %s",
                 kernels_.at(tinfo.hash_code()).get_name());
      return kernels_.at(tinfo.hash_code());
    }

    string_class name = get_kernel_name_from_class(tinfo);
    DEBUG_INFO("kernel class: %s", name.c_str());
    kernel k = task_handler->create_kernel(name.c_str());
    // shared_ptr_class<kernel> p(std::move(k));
    kernels_.insert(std::make_pair(tinfo.hash_code(), k));
    return k;
  }

  virtual bool open()     = 0;

  bool is_valid() {
    return task_handler.get() != nullptr;
  }

  device bound_device;
  task_handler_ptr task_handler;
  kernel_map kernels_; // all exiting kernels in the context
};

class cpu_context_info : public context_info {
  void* dll_;

public:
  cpu_context_info(device d) : context_info(d), dll_(nullptr) { open(); }

  ~cpu_context_info() {
    if (dll_)
      dlclose(dll_);
  }

  bool open() {
    const char* env = getenv(ENV_KERNEL);
    string_class fn(env ? env : DEFAULT_LIB);
    dll_ = dlopen(fn.c_str(), RTLD_LAZY);
    if (!dll_) {
      DEBUG_INFO("dlopen failed: %s", dlerror());
      return false;
    }
    DEBUG_INFO("kernel lib loaded: %lx, %s", (size_t)dll_, fn.c_str());
    task_handler = task_handler_ptr(new task_handler_cpu(dll_));
    return true;
  }
};
} // namespace neosycl::sycl::detail
