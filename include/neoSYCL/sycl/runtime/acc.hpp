#pragma once
#include <CL/sycl.hpp>

namespace neosycl::sycl::rt {

template <typename T> struct acc_ {
  T* p_;
  size_t r_[6];

  inline T& operator[](size_t i) { return p_[i]; }
  inline T& operator[](neosycl::sycl::id<3>& i) {
    return p_[(i[2] * r_[1] + i[1]) * r_[0] + i[0]];
  }
  inline T& operator[](neosycl::sycl::id<2>& i) { return p_[i[1] * r_[0] + i[0]]; }
  inline T& operator[](neosycl::sycl::id<1>& i) { return p_[i[0]]; }
};

} // namespace neosycl::sycl::rt
