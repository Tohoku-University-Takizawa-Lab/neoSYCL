#ifndef CUSTOM_SYCL_INCLUDE_SYCL_INFO_QUEUE_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_INFO_QUEUE_HPP_

namespace neosycl::sycl::info {

using queue_profiling = bool;

enum class queue : int { context, device, reference_count, properties };

} // namespace neosycl::sycl::info

#endif // CUSTOM_SYCL_INCLUDE_SYCL_INFO_QUEUE_HPP_
