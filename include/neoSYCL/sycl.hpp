#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_HPP_
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_HPP_

// debug headers
#include "sycl/detail/debug.hpp"
#include "sycl/types.hpp"

#include "sycl/exception.hpp"

// include device headers
#include "sycl/id.hpp"
#include "sycl/device.hpp"

// include device selector headers
#include "sycl/device_selector.hpp"
#include "sycl/device_selector/cpu_selector.hpp"


// include platform headers
#include "sycl/platform.hpp"

// include buffer headers
#include "sycl/accessor.hpp"
#include "sycl/accessor.hpp"
#include "sycl/buffer.hpp"

#include "sycl/queue.hpp"
#include "sycl/handler.hpp"

namespace neosycl {

using namespace neosycl::sycl;

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_HPP_
