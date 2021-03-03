#ifndef CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_

#include <utility>
#include <shared_mutex>

#include "nd_range.hpp"
#include "types.hpp"
#include "event.hpp"
#include "id.hpp"
#include "allocator.hpp"
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
public:
  template<typename KernelName, typename KernelType>
  void single_task(KernelType kernelFunc) {}

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

  //----- OpenCL interoperability interface //
  template<typename T>
  void set_arg(int argIndex, T &&arg);

  template<typename... Ts>
  void set_args(Ts &&... args);
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_HANDLER_HPP_
