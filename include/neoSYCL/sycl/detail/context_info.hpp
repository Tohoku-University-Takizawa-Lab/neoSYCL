#pragma once

namespace neosycl::sycl::detail {
class context_info {
  using container_type = std::shared_ptr<detail::container::DataContainer>;

protected:
  using handler_type = shared_ptr_class<detail::task_handler>;
  context_info() {}

public:
  virtual ~context_info() = default;
  void *get_pointer(container_type c) const {
    return task_handler->get_pointer(c);
  }
  void *alloc_mem(container_type c, access::mode m = access::mode::read) const {
    return task_handler->alloc_mem(c, m);
  }
  void free_mem(container_type c) const { task_handler->free_mem(c); }

  handler_type task_handler;
};

class cpu_context_info : public context_info {
public:
  cpu_context_info() : context_info() {
    task_handler = handler_type(new task_handler_cpu());
  }
  ~cpu_context_info() = default;
};
} // namespace neosycl::sycl::detail