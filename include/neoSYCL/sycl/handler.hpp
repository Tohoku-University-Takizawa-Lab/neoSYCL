#ifndef CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_

#include <utility>
#include <shared_mutex>

#include "neoSYCL/sycl/nd_range.hpp"
#include "neoSYCL/sycl/types.hpp"
#include "neoSYCL/sycl/event.hpp"
#include "neoSYCL/sycl/id.hpp"
#include "neoSYCL/sycl/allocator.hpp"
#include "neoSYCL/sycl/detail/highlight_func.hpp"
#include "neoSYCL/sycl/detail/kernel.hpp"
#include "neoSYCL/sycl/detail/task.hpp"
#include "neoSYCL/sycl/detail/task_handler.hpp"
#include "neoSYCL/sycl/detail/registered_platforms.hpp"
#include "neoSYCL/sycl/detail/task_counter.hpp"

namespace neosycl::sycl {

namespace detail {

/**
 * get func name by create ptr, rely on compiler reflect implementation
 *
 * @tparam KernelName class
 * @return str
 */
template <typename KernelName> string_class get_kernel_name_from_class() {
  KernelName *p;
  string_class in = typeid(p).name();
  return in;
}

} // namespace detail

class handler {
public:
  explicit handler(device dev, shared_ptr_class<detail::task_counter> counter)
      : bind_device(std::move(dev)), counter(std::move(counter)),
        kernel(new detail::kernel()) {}

  template <typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc) {
    kernel->name = detail::get_kernel_name_from_class<KernelName>();
    shared_ptr_class<detail::task_handler> handler =
        detail::PLATFORM_HANDLER_MAP[bind_device.device_info->type()];
    submit_task(
        [f = kernelFunc, h = handler, k = kernel]() { h->single_task(k, f); });
  }

  template <typename KernelType>
  void submit_parallel_for(shared_ptr_class<detail::task_handler> handler,
                           range<3> numWorkItems, id<3> offset,
                           KernelType kernelFunc) {
    submit_task([f = kernelFunc, n = numWorkItems, o = offset,
                 h = std::move(handler),
                 k = kernel]() { h->parallel_for_3d(k, n, f, o); });
  }

  template <typename KernelType>
  void submit_parallel_for(shared_ptr_class<detail::task_handler> handler,
                           range<2> numWorkItems, id<2> offset,
                           KernelType kernelFunc) {
    submit_task([f = kernelFunc, n = numWorkItems, o = offset,
                 h = std::move(handler),
                 k = kernel]() { h->parallel_for_2d(k, n, f, o); });
  }

  template <typename KernelType>
  void submit_parallel_for(shared_ptr_class<detail::task_handler> handler,
                           range<1> numWorkItems, id<1> offset,
                           KernelType kernelFunc) {
    submit_task([f = kernelFunc, n = numWorkItems, o = offset,
                 h = std::move(handler),
                 k = kernel]() { h->parallel_for_1d(k, n, f, o); });
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems, KernelType kernelFunc) {
    kernel->name = detail::get_kernel_name_from_class<KernelName>();
    shared_ptr_class<detail::task_handler> handler =
        detail::PLATFORM_HANDLER_MAP[bind_device.device_info->type()];
    submit_parallel_for(handler, numWorkItems, id<dimensions>(), kernelFunc);
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems,
                    id<dimensions> workItemOffset, KernelType kernelFunc) {
    kernel->name = detail::get_kernel_name_from_class<KernelName>();
    shared_ptr_class<detail::task_handler> handler =
        detail::PLATFORM_HANDLER_MAP[bind_device.device_info->type()];
    submit_parallel_for(handler, numWorkItems, workItemOffset, kernelFunc);
  }

  //  template<typename KernelName, typename KernelType, int dimensions>
  //  void parallel_for(nd_range<dimensions> executionRange, KernelType
  //  kernelFunc);

  template <typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               WorkgroupFunctionType kernelFunc);

  template <typename KernelName, typename WorkgroupFunctionType, int dimensions>
  void parallel_for_work_group(range<dimensions> numWorkGroups,
                               range<dimensions> workGroupSize,
                               WorkgroupFunctionType kernelFunc);

  //----- OpenCL interoperability interface //
  template <typename T> void set_arg(int argIndex, T &&arg) {
    kernel->args.insert(argIndex, arg);
  }

  template <typename... Ts> void set_args(Ts &&...args) {
    kernel->args.push_back(args...);
  }

  shared_ptr_class<detail::kernel> get_kernel() { return kernel; }

private:
  shared_ptr_class<detail::kernel> kernel;
  device bind_device;
  shared_ptr_class<detail::task_counter> counter;

  template <typename Func> void submit_task(Func func) {
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

} // namespace neosycl::sycl

#endif // CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
