#pragma once
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <iostream>

namespace neosycl::sycl {

template <class T, class Alloc = std::allocator<T>>
using vector_class = std::vector<T, Alloc>;

using string_class = std::string;

template <class R, class... ArgTypes>
using function_class = std::function<R(ArgTypes...)>;

using mutex_class = std::mutex;

template <class T>
using shared_ptr_class = std::shared_ptr<T>;

template <class T>
using unique_ptr_class = std::unique_ptr<T>;

template <class T>
using weak_ptr_class = std::weak_ptr<T>;

template <class T>
using hash_class = std::hash<T>;

using exception_ptr_class = std::exception_ptr;

template <class T>
using buffer_allocator = std::allocator<T>;

/* OpenCL interop (minimum type definition only) */
using cl_platform_id = u_int64_t;
using cl_device_id   = u_int64_t;
using cl_context     = u_int64_t;
using cl_program     = u_int64_t;
using cl_kernel      = u_int64_t;
} // namespace neosycl::sycl
