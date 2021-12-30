#pragma once

namespace neosycl::sycl::detail {
class context_info {
protected:
  using handler_type = shared_ptr_class<detail::task_handler>;
  context_info() {}

public:
  ~context_info() = default;
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