#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP

#include "neoSYCL/sycl/detail/kernel.hpp"

namespace neosycl::sycl::detail {

class task_handler {
protected:
  using container_type = std::shared_ptr<detail::container::DataContainer>;

public:
  explicit task_handler() {}

  virtual void single_task(shared_ptr_class<kernel> k,
                           const std::function<void(void)> &func) = 0;

  virtual void parallel_for_1d(shared_ptr_class<kernel> k, range<1> r,
                               const std::function<void(id<1>)> &func,
                               id<1> offset) = 0;

  virtual void parallel_for_2d(shared_ptr_class<kernel> k, range<2> r,
                               const std::function<void(id<2>)> &func,
                               id<2> offset) = 0;

  virtual void parallel_for_3d(shared_ptr_class<kernel> k, range<3> r,
                               const std::function<void(id<3>)> &func,
                               id<3> offset) = 0;

  virtual SUPPORT_PLATFORM_TYPE type() = 0;

  virtual void *get_pointer(container_type)                  = 0;
  virtual void *alloc_mem(container_type,
                          access::mode = access::mode::read) = 0;
  virtual void free_mem(container_type)                      = 0;
  virtual void copy_back()                                   = 0;
};

class task_handler_cpu : public task_handler {
public:
  task_handler_cpu() {}

  void single_task(shared_ptr_class<kernel> k,
                   const std::function<void(void)> &func) override {
    for (const accessor_info &arg : k->args) {
      arg.acquire_access();
    }
    DEBUG_INFO("execute single %d kernel, name: %s\n", type(), k->name.c_str());
    func();
    for (const accessor_info &arg : k->args) {
      arg.release_access();
    }
  }

  void parallel_for_1d(shared_ptr_class<kernel> k, range<1> r,
                       const std::function<void(id<1>)> &func,
                       id<1> offset) override {
    for (const accessor_info &arg : k->args) {
      arg.acquire_access();
    }
    for (size_t x = offset.get(0); x < r.get(0); x++) {
      func(id<1>(x));
    }
    for (const accessor_info &arg : k->args) {
      arg.release_access();
    }
  };

  void parallel_for_2d(shared_ptr_class<kernel> k, range<2> r,
                       const std::function<void(id<2>)> &func,
                       id<2> offset) override {
    for (const accessor_info &arg : k->args) {
      arg.acquire_access();
    }
    for (size_t x = offset.get(0); x < r.get(0); x++) {
      for (size_t y = offset.get(1); y < r.get(1); y++) {
        func(id<2>(x, y));
      }
    }
    for (const accessor_info &arg : k->args) {
      arg.release_access();
    }
  };

  void parallel_for_3d(shared_ptr_class<kernel> k, range<3> r,
                       const std::function<void(id<3>)> &func,
                       id<3> offset) override {
    for (const accessor_info &arg : k->args) {
      arg.acquire_access();
    }
    for (size_t x = offset.get(0); x < r.get(0); x++) {
      for (size_t y = offset.get(1); y < r.get(1); y++) {
        for (size_t z = offset.get(2); z < r.get(2); z++) {
          func(id<3>(x, y, z));
        }
      }
    }
    for (const accessor_info &arg : k->args) {
      arg.release_access();
    }
  };

  SUPPORT_PLATFORM_TYPE type() override { return CPU; }

  void *get_pointer(container_type p) override { return p->get_raw_ptr(); }
  void *alloc_mem(container_type p, access::mode = access::mode::read) override {
    return p->get_raw_ptr();
  }
  void free_mem(container_type) override {}
  void copy_back() override {}
};

} // namespace neosycl::sycl::detail

#endif // NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP
