#ifndef SYCL_INCLUDE_CL_SYCL_ALLOCATOR_HPP_
#define SYCL_INCLUDE_CL_SYCL_ALLOCATOR_HPP_

namespace neosycl::sycl {

template <typename T> using buffer_allocator = std::allocator<T>;

}

#endif // SYCL_INCLUDE_CL_SYCL_ALLOCATOR_HPP_
