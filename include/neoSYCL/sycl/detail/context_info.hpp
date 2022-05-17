#pragma once
#include <map>
#include <regex>
#include <cxxabi.h>

namespace neosycl::sycl::detail {
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
  using container_type = std::shared_ptr<detail::container::DataContainer>;

protected:
  using handler_type = shared_ptr_class<detail::task_handler>;
  using kernel_map   = std::map<size_t, shared_ptr_class<kernel>>;

  context_info() {}

public:
  virtual ~context_info() = default;
  void* get_pointer(container_type c) const {
    return task_handler->get_pointer(c);
  }
  void* alloc_mem(container_type c, access::mode m = access::mode::read) const {
    return task_handler->alloc_mem(c, m);
  }
  void free_mem(container_type c) const { task_handler->free_mem(c); }

  void set_capture(shared_ptr_class<kernel> k, void* p, size_t sz) const {
    task_handler->set_capture(k, p, sz);
  }

  template <typename KernelName>
  shared_ptr_class<kernel> get_kernel() {
    const std::type_info& tinfo = typeid(KernelName*);

    if(kernels_.count(tinfo.hash_code()))
      return kernels_.at(tinfo.hash_code());

    string_class name  = get_kernel_name_from_class(tinfo);
    DEBUG_INFO("kernel class: %s", name.c_str());
    kernel* k = task_handler->create_kernel(name.c_str());
    shared_ptr_class<kernel> p(std::move(k));
    kernels_.insert(std::make_pair(tinfo.hash_code(), p));
    return p;
  }

  handler_type task_handler;
  kernel_map kernels_; // all exiting kernels in the context
};

class cpu_context_info : public context_info {
public:
  cpu_context_info() : context_info() {
    task_handler = handler_type(new task_handler_cpu());
  }
  ~cpu_context_info() = default;
};
} // namespace neosycl::sycl::detail
