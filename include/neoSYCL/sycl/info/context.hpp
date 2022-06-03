#pragma once

namespace neosycl::sycl::info {

using gl_context_interop = bool;

// enum class context : int { reference_count, num_devices, gl_interop };
enum class context : int { reference_count, platform, devices };

} // namespace neosycl::sycl::info
