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
    submit_task([f = kernelFunc, c = counter, h = handler, k = kernel]() {
      h->single_task(k, f);
    });
  }

  template<typename KernelName, typename KernelType>
  void parallel_for(range<1> numWorkItems, KernelType kernelFunc) {
    kernel.name = detail::get_kernel_name_from_class<KernelName>();
  }

  template<typename KernelName, typename KernelType, int dimensions>
  void parallel_for(range<dimensions> numWorkItems, id<dimensions> workItemOffset, KernelType kernelFunc) {
    kernel.name = detail::get_kernel_name_from_class<KernelName>();

  }

  template<typename KernelName, typename KernelType, int dimensions>
  void parallel_for(nd_range<dimensions> executionRange, KernelType kernelFunc) {
    kernel.name = detail::get_kernel_name_from_class<KernelName>();

  }

  template<typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups, WorkgroupFunctionType kernelFunc) {
    kernel.name = detail::get_kernel_name_from_class<KernelName>();

  }

  template<typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               range<dimensions> workGroupSize,
                               WorkgroupFunctionType kernelFunc) {
    kernel.name = detail::get_kernel_name_from_class<KernelName>();
  }

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
