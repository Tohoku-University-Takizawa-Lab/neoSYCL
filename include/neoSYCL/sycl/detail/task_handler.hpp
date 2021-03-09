#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP

#include "kernel.hpp"

namespace neosycl::sycl::detail {

struct task_handler {

  virtual void single_task(kernel k, const std::function<void(void)> &func) = 0;

  virtual void parallel_for_1d(kernel k,
                               range<1> r,
                               const std::function<void(id<1>)> &func,
                               id<1> offset) = 0;

  virtual void parallel_for_2d(kernel k,
                               range<2> r,
                               const std::function<void(id<2>)> &func,
                               id<2> offset) = 0;

  virtual void parallel_for_3d(kernel k,
                               range<3> r,
                               const std::function<void(id<3>)> &func,
                               id<3> offset) = 0;

  virtual SUPPORT_PLATFORM_TYPE type() = 0;

};

struct task_handler_cpu : public task_handler {

  void single_task(kernel k, const std::function<void(void)> &func) override {
    DEBUG_INFO("execute single {} kernel, name: {}", int (type()), k.name);
    func();
  }

  void parallel_for_1d(kernel k,
                       range<1> r,
                       const std::function<void(id<1>)> &func,
                       id<1> offset) override {
    for (size_t x = offset.get(0); x < r.get(0); x++) {
      func(id<1>(x));
    }
  };

  void parallel_for_2d(kernel k,
                       range<2> r,
                       const std::function<void(id<2>)> &func,
                       id<2> offset) override {
    for (size_t x = offset.get(0); x < r.get(0); x++) {
      for (size_t y = offset.get(1); y < r.get(1); y++) {
        func(id<2>(x, y));
      }
    }
  };

  void parallel_for_3d(kernel k,
                       range<3> r,
                       const std::function<void(id<3>)> &func,
                       id<3> offset) override {
    for (size_t x = offset.get(0); x < r.get(0); x++) {
      for (size_t y = offset.get(1); y < r.get(1); y++) {
        for (size_t z = offset.get(2); z < r.get(2); z++) {
          func(id<3>(x, y, z));
        }
      }
    }
  };

  SUPPORT_PLATFORM_TYPE type() override {
    return CPU;
  }

};

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_TASK_HANDLER_HPP
