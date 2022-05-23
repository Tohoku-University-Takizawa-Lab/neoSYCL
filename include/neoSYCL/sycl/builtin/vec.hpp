#pragma once

namespace neosycl::sycl {

template <typename T, size_t NumElems> class vec {
public:
  using element_type = T;
  element_type data_[NumElems];

  static constexpr size_t size() { return NumElems; }

  vec()           = default;
  vec(const vec&) = default;
  vec(vec&& Rhs)  = default;

  explicit vec(const T& x) {
    for (size_t i(0); i < size(); i++)
      data_[i] = x;
  }

  template <size_t N = NumElems, typename = std::enable_if_t<(N == 1)>>
  vec(const T& x) {
    for (size_t i(0); i < size(); i++)
      data_[i] = x;
  }
  template <size_t N = size(), typename = std::enable_if_t<(N == 2)>>
  vec(const T& x, const T& y) {
    data_[0] = x;
    data_[1] = y;
  }
  template <size_t N = size(), typename = std::enable_if_t<(N == 3)>>
  vec(const T& x, const T& y, const T& z) {
    data_[0] = x;
    data_[1] = y;
    data_[2] = z;
  }
  template <size_t N = size(), typename = std::enable_if_t<(N == 4)>>
  vec(const T& x, const T& y, const T& z, const T& w) {
    data_[0] = x;
    data_[1] = y;
    data_[2] = z;
    data_[3] = w;
  }

  vec& operator=(const vec& Rhs) = default;

  vec& operator=(const T& Rhs) {
    for (size_t i = 0; i < size(); i++)
      data_[i] = Rhs;
  }

  vec& operator+=(const vec& Rhs) {
    *this = *this + Rhs;
    return *this;
  };

  element_type& x() { return data_[0]; }
  template <size_t N = size(), typename = std::enable_if_t<(N > 1)>>
  element_type& y() {
    return data_[1];
  }
  template <size_t N = size(), typename = std::enable_if_t<(N > 2)>>
  element_type& z() {
    return data_[2];
  }
  template <size_t N = size(), typename = std::enable_if_t<(N > 3)>>
  element_type& w() {
    return data_[3];
  }
  inline element_type& operator[](size_t i) { return data_[i]; }
  inline const element_type operator[](size_t i) const { return data_[i]; }
};
template <typename T, size_t D>
vec<T, D> operator+(const vec<T, D>& x, const vec<T, D>& y) {
  vec<T, D> r;
  for (size_t i(0); i < D; i++)
    r[i] = x[i] + y[i];
  return r;
}

template <typename T, size_t D>
vec<T, D> operator*(const vec<T, D>& x, const vec<T, D>& y) {
  vec<T, D> r;
  for (size_t i(0); i < D; i++)
    r[i] = x[i] * y[i];
  return r;
}
template <typename T, size_t D>
vec<T, D> operator*(const vec<T, D>& x, const T& y) {
  vec<T, D> r;
  for (size_t i(0); i < D; i++)
    r[i] = x[i] * y;
  return r;
}
template <typename T, size_t D>
vec<T, D> operator*(const T& x, const vec<T, D>& y) {
  return operator*(y, x);
}

using float4     = vec<float, 4>;
using float3     = vec<float, 3>;
using float2     = vec<float, 2>;
using double4    = vec<double, 4>;
using double3    = vec<double, 3>;
using double2    = vec<double, 2>;
using int4       = vec<int, 4>;
using int3       = vec<int, 3>;
using int2       = vec<int, 2>;
using uint4      = vec<uint, 4>;
using uint3      = vec<uint, 3>;
using uint2      = vec<uint, 2>;
using ulonglong  = unsigned long long;
using ulonglong4 = vec<ulonglong, 4>;
using ulonglong3 = vec<ulonglong, 3>;
using ulonglong2 = vec<ulonglong, 2>;
} // namespace neosycl::sycl
