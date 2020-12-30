#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_HPP_
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_HPP_

// debug headers
#include "sycl/debug.hpp"

// include device headers
#include "sycl/device.hpp"
#include "sycl/id.hpp"

// include device selector headers
#include "sycl/device_selector.hpp"
#include "sycl/device_selector/default_selector.hpp"
#include "sycl/device_selector/cpu_selector.hpp"
#include "sycl/device_selector/accelerator_selector.hpp"
#include "sycl/device_selector/gpu_selector.hpp"
#include "sycl/device_selector/host_selector.hpp"

// include platform headers
#include "sycl/platform.hpp"

// include buffer headers
#include "sycl/buffer.hpp"
#include "sycl/accessor.hpp"

#include "sycl/queue.hpp"
#include "sycl/handler.hpp"

#ifdef BUILD_VE
#include "neoSYCL/sycl/nec/ve_device.hpp"
#include "neoSYCL/sycl/nec/ve_queue.hpp"
#include "neoSYCL/sycl/nec/ve_task.hpp"
#include "neoSYCL/sycl/nec/ve_kernel.hpp"
#endif

namespace neosycl {

using namespace neosycl::sycl;

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_HPP_
