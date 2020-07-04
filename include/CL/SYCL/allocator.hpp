//
// Created by WhiteBlue on 2020/5/29.
//

#ifndef SYCL_INCLUDE_CL_SYCL_ALLOCATOR_HPP_
#define SYCL_INCLUDE_CL_SYCL_ALLOCATOR_HPP_

namespace cl::sycl {

template <typename T>
using buffer_allocator = std::allocator<T>;

}

#endif //SYCL_INCLUDE_CL_SYCL_ALLOCATOR_HPP_
