#ifndef CUSTOM_SYCL_INCLUDE_SYCL_INFO_CONTEXT_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_INFO_CONTEXT_HPP_

namespace neosycl::sycl::info {

using gl_context_interop = bool;

enum class context : int { reference_count, num_devices, gl_interop };

} // namespace neosycl::sycl::info

#endif // CUSTOM_SYCL_INCLUDE_SYCL_INFO_CONTEXT_HPP_
