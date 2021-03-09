#ifndef CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_

#include <utility>
#include <shared_mutex>

#include "nd_range.hpp"
#include "types.hpp"
#include "event.hpp"
#include "id.hpp"
#include "allocator.hpp"
#include "detail/highlight_func.hpp"
#include "detail/kernel.hpp"
#include "detail/task.hpp"
#include "detail/task_handler.hpp"
#include "detail/registered_platforms.hpp"
#include "detail/task_counter.hpp"

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
public:
  explicit handler(device dev,
                   shared_ptr_class<detail::task_counter> counter) :
      bind_device(std::move(dev)), counter(std::move(counter)) {}

  template<typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc) {
    kernel.name = detail::get_kernel_name_from_class<KernelName>();
    shared_ptr_class<detail::task_handler> handler = detail::PLATFORM_HANDLER_MAP[bind_device.device_info->type()];
    submit_task([f = kernelFunc, h = handler, k = kernel]() {
      h->single_task(k, f);
    });
  }

  template<typename KernelType, int dimensions>
  void submit_parallel_for(shared_ptr_class<detail::task_handler> handler,
                           range<dimensions> numWorkItems,
                           id<dimensions> offset,
                           KernelType kernelFunc) {
    submit_task([f = kernelFunc, n = numWorkItems, o = offset, h = std::move(handler), k = kernel]() {
      switch (dimensions) {
      case 1:h->parallel_for_1d(k, n, o, f);
        break;
      case 2:h->parallel_for_2d(k, n, o, f);
        break;
      case 3:h->parallel_for_3d(k, n, o, f);
        break;
      default:throw feature_not_supported("unsupported dimensions");
      }
    });
  }

  template<typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems, KernelType kernelFunc) {
    kernel.name = detail::get_kernel_name_from_class<KernelName>();
    shared_ptr_class<detail::task_handler> handler = detail::PLATFORM_HANDLER_MAP[bind_device.device_info->type()];
    submit_parallel_for(handler, numWorkItems, id<dimensions>(), kernelFunc);
  }

  template<typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems, id<dimensions> workItemOffset, KernelType kernelFunc) {
    kernel.name = detail::get_kernel_name_from_class<KernelName>();
    shared_ptr_class<detail::task_handler> handler = detail::PLATFORM_HANDLER_MAP[bind_device.device_info->type()];
    submit_parallel_for(handler, numWorkItems, workItemOffset, kernelFunc);
  }

//  template<typename KernelName, typename KernelType, int dimensions>
//  void parallel_for(nd_range<dimensions> executionRange, KernelType kernelFunc);

  template<typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups, WorkgroupFunctionType kernelFunc);

  template<typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               range<dimensions> workGroupSize,
                               WorkgroupFunctionType kernelFunc);

  //----- OpenCL interoperability interface //
  template<typename T>
  void set_arg(int argIndex, T &&arg) {
    kernel.args.insert(argIndex, arg);
  }

  template<typename... Ts>
  void set_args(Ts &&... args) {
    kernel.args.push_back(args...);
  }

private:
  detail::kernel kernel;
  device bind_device;
  shared_ptr_class<detail::task_counter> counter;

  template<typename Func>
  void submit_task(Func func) {
    counter->incr();
    std::thread t([f = func, c = counter]() {
      try {
        f();
      } catch (...) {
        throw;
      }
      c->decr();
    });
    t.detach();
  }
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
