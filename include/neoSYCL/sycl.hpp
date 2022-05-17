#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_HPP_
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_HPP_

// debug headers
#include "sycl/detail/debug.hpp"

#include "sycl/types.hpp"
#include "sycl/exception.hpp"
#include "sycl/op_def.hpp"
#include "sycl/range.hpp"
#include "sycl/item.hpp"
#include "sycl/id.hpp"

#ifndef ___NEOSYCL_KERNEL_RUNTIME_ONLY___
#include "sycl/device_selector.hpp"
#include "sycl/platform.hpp"
#include "sycl/device.hpp"

#include "sycl/device_selector/cpu_selector.hpp"
#ifdef BUILD_VE
#include "extensions/nec/ve_selector.hpp"
#endif

#include "sycl/property_list.hpp"
#include "sycl/context.hpp"
#include "sycl/access.hpp"
#include "sycl/accessor.hpp"
#include "sycl/handler.hpp"
#include "sycl/queue.hpp"

// include buffer headers
#include "sycl/allocator.hpp"
#include "sycl/buffer.hpp"
#endif // ___NEOSYCL_KERNEL_RUNTIME_ONLY___

#ifdef BUILD_VE
#include "extensions/nec/rt_acc.hpp"
#endif

namespace neosycl {

using namespace neosycl::sycl;

}

#endif // NEOSYCL_INCLUDE_NEOSYCL_SYCL_HPP_
