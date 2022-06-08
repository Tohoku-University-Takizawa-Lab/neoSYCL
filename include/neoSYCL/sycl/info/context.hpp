#pragma once

namespace neosycl::sycl::info {

using gl_context_interop = bool;

// enum class context : int { reference_count, num_devices, gl_interop };
enum class context : int { reference_count, platform, devices };

DEF_INFO_TYPE_TRAIT(info::context, info::context::reference_count, long);
DEF_INFO_TYPE_TRAIT(info::context, info::context::platform, sycl::platform);
DEF_INFO_TYPE_TRAIT(info::context, info::context::devices,
                    vector_class<sycl::device>);

} // namespace neosycl::sycl::info
