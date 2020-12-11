#ifndef CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_

#include <utility>
#include <shared_mutex>

#include "nd_range.hpp"
#include "types.hpp"
#include "event.hpp"
#include "id.hpp"
#include "allocator.hpp"
#include "queue/task_queue.hpp"
#include "buffer/data_container.hpp"
#include "kernel/task.hpp"
#include "kernel/kernel.hpp"
#include "kernel/highlight_func.hpp"
#include "kernel.hpp"

namespace neosycl::sycl {

namespace detail {

/**
 * get func name by create ptr, rely on compiler reflect implementation
 *
 * @tparam KernelName class
 * @return str
 */
template<typename KernelName>
string_class get_kernel_name_from_class() {
  KernelName *p;
  string_class in = typeid(p).name();
  return in;
}

}

class handler {
 private:
  std::shared_ptr<detail::Task> task;
  std::shared_ptr<detail::TaskQueue> tq;

 public:

  template<typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc);

  template<typename KernelName, typename KernelType>
  void parallel_for(range<1> numWorkItems, KernelType kernelFunc);

  template<typename KernelName, typename KernelType, int dimensions>
  void parallel_for(range<dimensions> numWorkItems, id<dimensions> workItemOffset, KernelType kernelFunc);

  template<typename KernelName, typename KernelType, int dimensions>
  void parallel_for(nd_range<dimensions> executionRange, KernelType kernelFunc);

  template<typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups, WorkgroupFunctionType kernelFunc);

  template<typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               range<dimensions> workGroupSize,
                               WorkgroupFunctionType kernelFunc);

  void single_task(kernel syclKernel);

  template<int dimensions>
  void parallel_for(range<dimensions> numWorkItems, kernel syclKernel);

  template<int dimensions>
  void parallel_for(range<dimensions> numWorkItems, id<dimensions> workItemOffset, kernel syclKernel);

  template<int dimensions>
  void parallel_for(nd_range<dimensions> ndRange, kernel syclKernel);

  template<access::mode mode>
  void add_arg(std::shared_ptr<detail::DataContainer> arg) {
    detail::KernelArg kernel_arg(std::move(arg), mode);
    task->add_arg(kernel_arg);
  }

  void schedule(const std::function<void(void)> &f) {
    tq->thread_start();
    auto func = [tq = tq, args = task->args, f = f] {
#ifndef SYNC
      for (const detail::KernelArg &arg:args) {
        arg.lock();
        DEBUG_INFO("[handler] lock arg: {:#x}, mode: {}", (size_t) arg.container.get(), arg.mode);
      }
#endif

      f();

#ifndef SYNC
      for (const detail::KernelArg &arg:args) {
        arg.unlock();
        DEBUG_INFO("[handler] release arg: {:#x}, mode: {}", (size_t) arg.container.get(), arg.mode);
      }
#endif
      tq->thread_end();
    };

#ifdef SYNC
    func();
#else
    std::thread t(func);
    t.detach();
#endif

  }

  explicit handler(std::shared_ptr<detail::TaskQueue> tq)
      : tq(std::move(tq)), task(std::shared_ptr<detail::Task>(new detail::Task)) {}


  template<typename KernelName, typename ParallelForFunctor>
  void parallel_for(range<3> global_size, ParallelForFunctor f) {
    detail::HIGHLIGHT_KERNEL_PARALLEL<KernelName, ParallelForFunctor, 3>(f, global_size);

    auto func = [task = task, f = f, r = global_size]() {
      if (task->is_cpu()) {
        for (size_t x = 0; x < r.get(1); x++) {
          for (size_t y = 0; y < r.get(2); y++) {
            for (size_t z = 0; z < r.get(3); z++) {
              f(id<3>(x, y, z));
            }
          }
        }
      } else {
        string_class name_str = detail::get_kernel_name_from_class<KernelName>();
        task->get_kernel(name_str)->parallel_for(r);
      }
    };
    schedule(func);
  }
};

template<typename KernelName, typename KernelType>
void handler::single_task(KernelType f) {
  detail::HIGHLIGHT_KERNEL_SINGLE_TASK<KernelName, KernelType>(f);

  auto func = [task = task, f = f]() {
    if (task->is_cpu()) {
      f();
    } else {
      string_class name_str = detail::get_kernel_name_from_class<KernelName>();
      task->get_kernel(name_str)->single_task();
    }
  };
  schedule(func);
}

template<typename KernelName, typename ParallelForFunctor>
void handler::parallel_for(range<1> numWorkItems, ParallelForFunctor syclKernel) {
  detail::HIGHLIGHT_KERNEL_PARALLEL<KernelName, ParallelForFunctor, 1>(syclKernel, numWorkItems);

  auto func = [task = task, f = syclKernel, r = numWorkItems]() {
    if (task->is_cpu()) {
      for (size_t x = 0; x < r.get(1); x++) {
        f(id<1>(x));
      }
    } else {
      string_class name_str = detail::get_kernel_name_from_class<KernelName>();
      task->get_kernel(name_str)->parallel_for(r);
    }
  };
  schedule(func);
}

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
