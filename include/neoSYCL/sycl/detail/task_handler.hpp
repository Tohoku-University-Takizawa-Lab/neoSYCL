#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP

#include "kernel.hpp"

namespace neosycl::sycl::detail {

struct task_handler {

  virtual void single_task(kernel k, const std::function<void()> &func) = 0;

  virtual SUPPORT_PLATFORM_TYPE type() = 0;

};

struct task_handler_cpu : public task_handler {

  void single_task(kernel k, const std::function<void()> &func) override {
    DEBUG_INFO("execute kernel with type {}, name: {}", type(), k.name);
    func();
  }

  SUPPORT_PLATFORM_TYPE type() override {
    return CPU;
  }
};

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP
