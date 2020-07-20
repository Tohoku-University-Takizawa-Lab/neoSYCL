#ifndef CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_

#include <utility>
#include <shared_mutex>

#include "CL/SYCL/types.hpp"
#include "CL/SYCL/event.hpp"
#include "CL/SYCL/id.hpp"
#include "CL/SYCL/allocator.hpp"
#include "CL/SYCL/queue/task_queue.hpp"
#include "CL/SYCL/buffer/data_container.hpp"
#include "CL/SYCL/kernel/task.hpp"
#include "CL/SYCL/kernel/kernel.hpp"
#include "CL/SYCL/kernel/highlight_func.hpp"

namespace cl::sycl {

namespace detail {

/**
 * get func name by create ptr, rely on compiler reflect implementation
 * @todo have better solution?
 *
 * @tparam KernelName class
 * @return str
 */
template<typename KernelName>
string_class get_kernel_name_from_class() {
  KernelName *p;
  string_class in = typeid(p).name();

  int index = 0;
  for (int i = 0; i < in.size(); i++) {
    char curr = in[i];
    if ('0' <= curr && curr <= '9') {
      index = i;
    }
  }
  return in.substr(index + 1, in.size() - index);
}

}

class handler {
 private:
  std::shared_ptr<detail::Task> task;
  std::shared_ptr<detail::TaskQueue> tq;

 public:

  template<access::mode mode>
  void add_arg(std::shared_ptr<detail::DataContainer> arg) {
    detail::KernelArg kernel_arg(arg, mode);
    task->add_arg(kernel_arg);
  }

  void schedule(const std::function<void(void)> &f) {
    tq->thread_start();
    auto func = [tq = tq, args = task->args, f = f] {
      for (detail::KernelArg arg:args) {
        arg.lock();
        DEBUG_INFO("[handler] lock arg: {:#x}, mode: {}", (size_t) arg.container.get(), arg.mode);
      }

      f();

      for (detail::KernelArg arg:args) {
        arg.unlock();
        DEBUG_INFO("[handler] release arg: {:#x}, mode: {}", (size_t) arg.container.get(), arg.mode);
      }
      tq->thread_end();
    };

#ifdef SYNC
    func();
#else
    std::thread t(func);
    t.detach();
#endif

  }

  handler(std::shared_ptr<detail::TaskQueue> tq, detail::Task *task) : tq(std::move(tq)), task(task) {}

  template<typename KernelName, typename ParallelForFunctor>
  void single_task(ParallelForFunctor f) {
    detail::HIGHLIGHT_KERNEL_SINGLE_TASK<KernelName, ParallelForFunctor>(f);

    auto func = [task = task, f = f]() {
      if (task->is_cpu()) {
        f();
      } else {
        string_class name_str = KernelName::name();
        task->get_kernel(name_str)->single_task();
      }
    };
    schedule(func);
  }

  template<typename KernelName, typename ParallelForFunctor>
  void parallel_for(range<1> global_size, ParallelForFunctor f) {
    detail::HIGHLIGHT_KERNEL_PARALLEL<KernelName, ParallelForFunctor, 1>(f, global_size);

    auto func = [task = task, f = f, r = global_size]() {
      if (task->is_cpu()) {
        for (size_t x = 0; x < r.get(1); x++) {
          f(id<1>(x));
        }
      } else {
        string_class name_str = KernelName::name();
        task->get_kernel(name_str)->parallel_for(r);
      }
    };
    schedule(func);
  }

  template<typename KernelName, typename ParallelForFunctor>
  void parallel_for(range<2> global_size, ParallelForFunctor f) {
    detail::HIGHLIGHT_KERNEL_PARALLEL<KernelName, ParallelForFunctor, 2>(f, global_size);

    auto func = [task = task, f = f, r = global_size]() {
      if (task->is_cpu()) {
        for (size_t x = 0; x < r.get(1); x++) {
          for (size_t y = 0; y < r.get(2); y++) {
            f(id<2>(x, y));
          }
        }
      } else {
        string_class name_str = KernelName::name();
        task->get_kernel(name_str)->parallel_for(r);
      }
    };
    schedule(func);
  }

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
        string_class name_str = KernelName::name();
        task->get_kernel(name_str)->parallel_for(r);
      }
    };
    schedule(func);
  }
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
