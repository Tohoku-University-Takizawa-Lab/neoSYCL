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

using half      = float; // TODO: is there any other option?
using ulonglong = unsigned long long;
using longlong  = long long;
using uchar     = unsigned char;
using schar     = signed char;

/* OpenCL interop (minimum type definition only) */
using cl_platform_id = u_int64_t;
using cl_device_id   = u_int64_t;
using cl_context     = u_int64_t;
using cl_program     = u_int64_t;
using cl_kernel      = u_int64_t;

using cl_char   = char;
using cl_uchar  = unsigned char;
using cl_schar  = signed char;
using cl_short  = short;
using cl_ushort = unsigned short;
using cl_int    = int;
using cl_uint   = uint;
using cl_long   = long;
using cl_ulong  = unsigned long;
using cl_float  = float;
using cl_double = double;
using cl_half   = half;
} // namespace neosycl::sycl
