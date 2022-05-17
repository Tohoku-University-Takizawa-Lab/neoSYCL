#pragma once
#include <CL/sycl.hpp>

namespace neosycl::sycl::rt {

template <typename T> struct acc_ {
  T* p_;
  size_t r_[6];

  acc_(T* p = nullptr) : p_(p), r_{1, 1, 1, 0, 0, 0} {}
  acc_(T* p, neosycl::sycl::range<1> r) : p_(p), r_{r[0], 1, 1, 0, 0, 0} {}
  acc_(T* p, neosycl::sycl::range<2> r) : p_(p), r_{r[0], r[1], 1, 0, 0, 0} {}
  acc_(T* p, neosycl::sycl::range<3> r)
      : p_(p), r_{r[0], r[1], r[2], 0, 0, 0} {}
  ~acc_() = default;

  inline T& operator[](size_t i) { return p_[i]; }
  inline T& operator[](neosycl::sycl::id<3>& i) {
    return p_[(i[2] * r_[1] + i[1]) * r_[0] + i[0]];
  }
  inline T& operator[](neosycl::sycl::id<2>& i) {
    return p_[i[1] * r_[0] + i[0]];
  }
  inline T& operator[](neosycl::sycl::id<1>& i) { return p_[i[0]]; }
};

} // namespace neosycl::sycl::rt
