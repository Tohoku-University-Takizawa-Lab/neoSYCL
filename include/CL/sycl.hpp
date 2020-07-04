//
// Created by WhiteBlue on 2020/5/23.
//

#ifndef CUSTOM_SYCL_INCLUDE_SYCL_SYCL_H_
#define CUSTOM_SYCL_INCLUDE_SYCL_SYCL_H_

// debug headers
#include "CL/SYCL/debug.hpp"

// include device headers
#include "CL/SYCL/device.hpp"

// include device selector headers
#include "CL/SYCL/device_selector.hpp"
#include "CL/SYCL/device_selector/default_selector.hpp"
#include "CL/SYCL/device_selector/cpu_selector.hpp"
#include "CL/SYCL/device_selector/accelerator_selector.hpp"
#include "CL/SYCL/device_selector/gpu_selector.hpp"
#include "CL/SYCL/device_selector/host_selector.hpp"

// include platform headers
#include "CL/SYCL/platform.hpp"

// include buffer headers
#include "CL/SYCL/buffer.hpp"
#include "CL/SYCL/accessor.hpp"

#include "CL/SYCL/queue.hpp"
#include "CL/SYCL/handler.hpp"

#include "CL/SYCL/nec/ve_device.hpp"
#include "CL/SYCL/nec/ve_queue.hpp"
#include "CL/SYCL/nec/ve_task.hpp"
#include "CL/SYCL/nec/ve_kernel.hpp"

#endif //CUSTOM_SYCL_INCLUDE_SYCL_SYCL_H_

