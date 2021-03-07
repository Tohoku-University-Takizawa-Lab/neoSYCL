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
  handler() : device_type(detail::SUPPORT_PLATFORM_TYPE::CPU) {};

  explicit handler(detail::SUPPORT_PLATFORM_TYPE type) : device_type(type) {}

  template<typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc) {
    kernel.name = detail::get_kernel_name_from_class<KernelName>();
    shared_ptr_class<detail::task_handler> handler = detail::PLATFORM_HANDLER_MAP[device_type];
    handler->single_task(kernelFunc);
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
  detail::SUPPORT_PLATFORM_TYPE device_type;
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
