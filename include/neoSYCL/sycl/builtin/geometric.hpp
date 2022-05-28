#pragma once

namespace neosycl::sycl {
template <typename T> vec<T, 4> cross(const vec<T, 4> p0, const vec<T, 4>& p1) {
  return vec<T, 4>(p0[1] * p1[2] - p0[2] * p1[1], p0[2] * p1[0] - p0[0] * p1[2],
                   p0[0] * p1[1] - p0[1] * p1[0], 0);
}
template <typename T> vec<T, 3> cross(const vec<T, 3> p0, const vec<T, 3>& p1) {
  return vec<T, 3>(p0[1] * p1[2] - p0[2] * p1[1], p0[2] * p1[0] - p0[0] * p1[2],
                   p0[0] * p1[1] - p0[1] * p1[0]);
}

template <typename T, size_t D> T dot(const vec<T, D> p0, const vec<T, D>& p1) {
  T r = 0;
  for (size_t i(0); i < D; i++)
    r += p0[i] * p1[i];
  return r;
}
template <typename T, size_t D>
T distance(const vec<T, D> p0, const vec<T, D>& p1) {
  T r = 0;
  for (size_t i(0); i < D; i++) {
    T diff = p0[i] - p1[i];
    r += diff * diff;
  }
  return sqrt(r);
}
template <typename T, size_t D> T length(const vec<T, D> p) {
  T r = 0;
  for (size_t i(0); i < D; i++) {
    r += p[i] * p[i];
  }
  return sqrt(r);
}
template <typename T, size_t D> vec<T, D> normalize(const vec<T, D> p) {
  vec<T, D> r = 0;
  T len       = length(p);
  for (size_t i(0); i < D; i++) {
    r = p[i] / len;
  }
  return r;
}
template <size_t D>
float fast_distance(const vec<float, D> p0, const vec<float, D>& p1) {
  float r = 0;
  for (size_t i(0); i < D; i++) {
    float diff = p0[i] - p1[i];
    r += diff * diff;
  }
  return std::sqrt(r);
}
template <size_t D> float fast_length(const vec<float, D> p) {
  float r = 0;
  for (size_t i(0); i < D; i++) {
    r += p[i] * p[i];
  }
  return std::sqrt(r);
}
template <size_t D> vec<float, D> fast_normalize(const vec<float, D> p) {
  vec<float, D> r = 0;
  float sum       = 0;
  for (size_t i(0); i < D; i++) {
    sum += p[i] * p[i];
  }
  float rlen = detail::rsqrt(sum);
  for (size_t i(0); i < D; i++) {
    r = p[i] * rlen;
  }
  return r;
}

} // namespace neosycl::sycl
