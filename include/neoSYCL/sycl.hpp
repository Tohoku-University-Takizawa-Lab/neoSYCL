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

#include "sycl/detail/runtime/acc.hpp"
#include "sycl/detail/builtin/builtin.hpp"

#ifndef ___NEOSYCL_KERNEL_RUNTIME_ONLY___
#include "sycl/device_selector.hpp"
#include "sycl/platform.hpp"
#include "sycl/device.hpp"
#include "sycl/context.hpp"
#include "sycl/kernel.hpp"
#include "sycl/program.hpp"

#include "neoSYCL/sycl/detail/kernel.hpp"
#include "neoSYCL/sycl/detail/program.hpp"
#include "neoSYCL/sycl/detail/device.hpp"
#include "neoSYCL/sycl/detail/context.hpp"
#include "neoSYCL/sycl/detail/platform.hpp"

#include "sycl/device_selector/cpu_selector.hpp"
#if 0
#ifdef BUILD_VE
#include "extensions/nec/ve_selector.hpp"
#endif
#endif
#include "sycl/atomic.hpp"
#include "sycl/accessor.hpp"
#include "sycl/handler.hpp"
#include "sycl/queue.hpp"

// include buffer headers
#include "sycl/allocator.hpp"
#include "sycl/buffer.hpp"
#endif // ___NEOSYCL_KERNEL_RUNTIME_ONLY___

namespace neosycl {

using namespace neosycl::sycl;

}

#endif // NEOSYCL_INCLUDE_NEOSYCL_SYCL_HPP_
