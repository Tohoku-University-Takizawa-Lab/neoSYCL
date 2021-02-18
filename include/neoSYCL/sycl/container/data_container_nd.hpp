#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_ND_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_ND_HPP_

#include <shared_mutex>

namespace neosycl::sycl::detail {

template<typename T, int Dimensions, typename AllocatorT = buffer_allocator <T>>
class DataContainerND : public DataContainer {};

}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_ND_HPP_
