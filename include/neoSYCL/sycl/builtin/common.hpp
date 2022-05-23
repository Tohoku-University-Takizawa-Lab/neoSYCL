#pragma once

namespace neosycl::sycl {
template <typename T> T clamp(T x, T y, T z) {
  return (x < y ? y : (x > z ? z : x));
}
template <typename T, size_t D>
vec<T, D> clamp(vec<T, D> x, vec<T, D> y, vec<T, D> z) {
  vec<T, D> ret;
  for (size_t i(0); i < D; i++)
    ret[i] = clamp(x[i], y[i], z[i]);
  return ret;
}
template <typename T, size_t D> vec<T, D> clamp(vec<T, D> x, T y, T z) {
  vec<T, D> ret;
  for (size_t i(0); i < D; i++)
    ret[i] = clamp(x[i], y, z);
  return ret;
}

} // namespace neosycl::sycl
