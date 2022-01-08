#ifndef CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_

#include <utility>
#include <shared_mutex>
#include <regex>
#include "neoSYCL/sycl/nd_range.hpp"
#include "neoSYCL/sycl/types.hpp"
#include "neoSYCL/sycl/event.hpp"
#include "neoSYCL/sycl/id.hpp"
#include "neoSYCL/sycl/allocator.hpp"
//#include "neoSYCL/sycl/detail/highlight_func.hpp"
#include "neoSYCL/sycl/detail/kernel.hpp"
#include "neoSYCL/sycl/detail/task.hpp"
#include "neoSYCL/sycl/detail/task_handler.hpp"
#include "neoSYCL/sycl/detail/registered_platforms.hpp"
#include "neoSYCL/sycl/detail/task_counter.hpp"
#include <cxxabi.h>

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
  int status;
  char *pc = abi::__cxa_demangle(typeid(p).name(), 0, 0, &status);
  string_class in(pc);
  free(pc);
  std::regex re("([^\\s\\:]+)\\*$");
  std::smatch result;
  if (std::regex_search(in, result, re)) {
    in = result[1].str();
  }

  return in;
}

} // namespace detail

class handler {
  using handler_type = shared_ptr_class<detail::task_handler>;
  using counter_type = shared_ptr_class<detail::task_counter>;
  using kernel_type  = shared_ptr_class<detail::kernel>;

public:
  explicit handler(device dev, counter_type counter, context c)
      : bind_device(std::move(dev)), counter(std::move(counter)),
        kernel(new detail::kernel()), ctx(c) {}

  template <typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc) {
    kernel->name = detail::get_kernel_name_from_class<KernelName>();
    handler_type task_handler = ctx.get_context_info()->task_handler;
    submit_task([f = kernelFunc, h = task_handler, k = kernel]() {
      h->single_task(k, f);
    });
  }

  template <typename KernelType>
  void submit_parallel_for(handler_type handler, range<3> numWorkItems,
                           id<3> offset, KernelType kernelFunc) {
    submit_task([f = kernelFunc, n = numWorkItems, o = offset,
                 h = std::move(handler),
                 k = kernel]() { h->parallel_for_3d(k, n, f, o); });
  }

  template <typename KernelType>
  void submit_parallel_for(handler_type handler, range<2> numWorkItems,
                           id<2> offset, KernelType kernelFunc) {
    submit_task([f = kernelFunc, n = numWorkItems, o = offset,
                 h = std::move(handler),
                 k = kernel]() { h->parallel_for_2d(k, n, f, o); });
  }

  template <typename KernelType>
  void submit_parallel_for(handler_type handler, range<1> numWorkItems,
                           id<1> offset, KernelType kernelFunc) {
    submit_task([f = kernelFunc, n = numWorkItems, o = offset,
                 h = std::move(handler),
                 k = kernel]() { h->parallel_for_1d(k, n, f, o); });
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems, KernelType kernelFunc) {
    kernel->name = detail::get_kernel_name_from_class<KernelName>();
    handler_type task_handler = ctx.get_context_info()->task_handler;
    submit_parallel_for(task_handler, numWorkItems, id<dimensions>(),
                        kernelFunc);
  }

  template <typename KernelName, typename KernelType, size_t dimensions>
  void parallel_for(range<dimensions> numWorkItems,
                    id<dimensions> workItemOffset, KernelType kernelFunc) {
    kernel->name = detail::get_kernel_name_from_class<KernelName>();
    handler_type task_handler = ctx.get_context_info()->task_handler;
    submit_parallel_for(task_handler, numWorkItems, workItemOffset, kernelFunc);
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

  kernel_type get_kernel() { return kernel; }

  template <typename T, size_t D, access::mode m, access::target t>
  void require(sycl::accessor<T, D, m, t, access::placeholder::true_t> acc) {
    acc.handler_ = this;
    kernel->args.push_back(detail::accessor_info(acc, m));
  }

private:
  kernel_type kernel;
  device bind_device;
  counter_type counter;
  context ctx;

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
