#pragma once

namespace neosycl::sycl {
namespace detail {
template <typename T>
T clamp(T x, T y, T z) {
  return (x < y ? y : (x > z ? z : x));
}
template <typename T>
T degrees(T i) {
  return i * 180 / M_PI;
}
template <typename T>
T mix(T x, T y, T z) {
  return (x * z + (1 - z) * y);
}
template <typename T>
T radians(T i) {
  return i * M_PI / 180;
}
template <typename T>
T step(T e, T x) {
  return x < e ? 0 : 1;
}
template <typename T>
T smoothstep(T e1, T e2, T x) {
  return x < e1 ? 0 : (x > e2 ? 1 : (x - e1) / (e2 - e1));
}
template <typename T>
T max(T x, T y) {
  return x < y ? y : x;
}
template <typename T>
T min(T x, T y) {
  return x < y ? x : y;
}
template <typename T>
T sign(T x) {
  return x < 0 ? 1 : 0;
}
} // namespace detail

DEFINE_GEN_FUNC3(clamp, detail::clamp);
template <typename T, int D>
vec<T, D> clamp(vec<T, D> x, T y, T z) {
  vec<T, D> ret;
  for (int i(0); i < D; i++)
    ret[i] = clamp(x[i], y, z);
  return ret;
}

DEFINE_GEN_FUNC(degrees, detail::degrees);

DEFINE_GEN_FUNC2(max, detail::max);
template <typename T, int D>
vec<T, D> max(vec<T, D> x, T y) {
  vec<T, D> ret;
  for (int i(0); i < D; i++)
    ret[i] = detail::max(x[i], y);
  return ret;
}

DEFINE_GEN_FUNC2(min, detail::min);
template <typename T, int D>
vec<T, D> min(vec<T, D> x, T y) {
  vec<T, D> ret;
  for (int i(0); i < D; i++)
    ret[i] = detail::min(x[i], y);
  return ret;
}

DEFINE_GEN_FUNC3(mix, detail::mix);
template <typename T, int D>
vec<T, D> mix(vec<T, D> x, vec<T, D> y, T z) {
  vec<T, D> ret;
  for (int i(0); i < D; i++)
    ret[i] = detail::mix(x[i], y[i], z);
  return ret;
}

DEFINE_GEN_FUNC(radians, detail::radians);

DEFINE_GEN_FUNC2(step, detail::step);
template <typename T, int D>
vec<T, D> step(T e, vec<T, D> x) {
  vec<T, D> ret;
  for (int i(0); i < D; i++)
    ret[i] = detail::step(e, x[i]);
  return ret;
}

DEFINE_GEN_FUNC3(smoothstep, detail::smoothstep);
template <typename T, int D>
vec<T, D> smoothstep(T e1, T e2, vec<T, D> x) {
  vec<T, D> ret;
  for (int i(0); i < D; i++)
    ret[i] = detail::smoothstep(e1, e2, x[i]);
  return ret;
}

DEFINE_GEN_FUNC(sign, detail::sign);

} // namespace neosycl::sycl
