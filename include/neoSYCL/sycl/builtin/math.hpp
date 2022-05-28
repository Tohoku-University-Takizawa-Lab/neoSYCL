#pragma once
#include <cmath>
#include <limits>
#include "vec.hpp"

namespace neosycl::sycl {

namespace detail {
template <typename T> T acospi(T i) { return std::acos(i) / M_PI; }
template <typename T> T asinpi(T i) { return std::asin(i) / M_PI; }
template <typename T> T atanpi(T i) { return std::atan(i) / M_PI; }
template <typename T> T atan2pi(T i, T j) { return std::atan2(i, j) / M_PI; }
template <typename T> T cospi(T i) { return std::cos(M_PI * i); }
template <typename T> T divide(T i, T j) { return i / j; }
template <typename T> T exp10(T i) { return std::exp(i + std::log(10)); }
template <typename T> T mad(T i, T j, T k) { return i * j + k; }
template <typename T> T maxmag(T i, T j) {
  if (fabs(i) > fabs(j))
    return i;
  if (fabs(i) < fabs(j))
    return j;

  return fmax(i, j);
}
template <typename T> T minmag(T i, T j) {
  if (fabs(i) < fabs(j))
    return i;
  if (fabs(i) > fabs(j))
    return j;

  return fmin(i, j);
}

template <typename T> T powr(T i, T j) { return std::pow(i, j); }
template <typename T> T recip(T i) { return (T)1 / i; }
template <typename T> T rootn(T x, int i) { return pow(x, (T)1 / i); }
template <typename T> T rsqrt(T i) { return std::sqrt(-i); }
template <typename T> T sinpi(T i) { return std::sin(i) / M_PI; }
template <typename T> T tanpi(T i) { return std::tan(i) / M_PI; }

} // namespace detail

namespace naitive {
using detail::divide;
using detail::exp10;
using detail::powr;
using detail::recip;
using detail::rsqrt;
using std::cos;
using std::exp;
using std::exp2;
using std::log;
using std::log10;
using std::log2;
using std::sin;
using std::sqrt;
using std::tan;
}; // namespace naitive

DEFINE_GEN_FUNC(acos, std::acos);
DEFINE_GEN_FUNC(acosh, std::acosh);
DEFINE_GEN_FUNC(acospi, detail::acospi);
DEFINE_GEN_FUNC(asin, std::asin);
DEFINE_GEN_FUNC(asinh, std::asinh);
DEFINE_GEN_FUNC(asinpi, detail::asinpi);
DEFINE_GEN_FUNC(atan, std::atan);
DEFINE_GEN_FUNC2(atan2, std::atan2);
DEFINE_GEN_FUNC(atanh, std::atanh);
DEFINE_GEN_FUNC(atanpi, detail::atanpi);
DEFINE_GEN_FUNC2(atan2pi, detail::atan2pi);
DEFINE_GEN_FUNC(cbrt, std::cbrt);
DEFINE_GEN_FUNC(ceil, std::ceil);
DEFINE_GEN_FUNC2(copysign, std::copysign);
DEFINE_GEN_FUNC(cos, std::cos);
DEFINE_GEN_FUNC(cosh, std::cosh);
DEFINE_GEN_FUNC(cospi, detail::cospi);
DEFINE_GEN_FUNC2(divide, detail::divide);
DEFINE_GEN_FUNC(erfc, std::erfc);
DEFINE_GEN_FUNC(erf, std::erf);
DEFINE_GEN_FUNC(exp, std::exp);
DEFINE_GEN_FUNC(exp2, std::exp2);
DEFINE_GEN_FUNC(exp10, detail::exp10);
DEFINE_GEN_FUNC(expm1, std::expm1);
DEFINE_GEN_FUNC(fabs, std::fabs);
DEFINE_GEN_FUNC2(fdim, std::fdim);
DEFINE_GEN_FUNC(floor, std::floor);
DEFINE_GEN_FUNC3(fma, std::fma);
DEFINE_GEN_FUNC2(fmax, std::fmax);
template <typename T, size_t D> vec<T, D> fmax(const vec<T, D>& x, T y) {
  return fmax(x, y);
}
DEFINE_GEN_FUNC2(fmin, std::fmin);
template <typename T, size_t D> vec<T, D> fmin(const vec<T, D>& x, T y) {
  return fmin(x, y);
}
DEFINE_GEN_FUNC2(fmod, std::fmod);
// DEFINE_GEN_FUNC2(fract,  *);
template <typename T> T fract(const T& x, T* p) {
  *p = std::floor(x);
  return x - *p;
}
template <typename T, size_t D> vec<T, D> fract(const vec<T, D>& x, T* p) {
  vec<T, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = fract(x[i], &p[i]);
  return R;
}
// DEFINE_GEN_FUNC2(frexp, std::frexp);
template <typename T> T frexp(const T& x, int* p) { return std::frexp(x, p); }
template <typename T, size_t D> vec<T, D> frexp(const vec<T, D>& x, int* p) {
  vec<T, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = frexp(x[i], &p[i]);
  return R;
}

DEFINE_GEN_FUNC2(hypot, std::hypot);

// DEFINE_GEN_FUNC_2T(int, logb, std::logbf);
template <typename T> int ilogb(const T& x) { return std::ilogb(x); }
template <typename T> int logb(const T& x) { return ilogb(x); }
template <typename T, size_t D> vec<int, D> ilogb(const vec<T, D>& x) {
  vec<int, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = ilogb(x[i]);
  return R;
}
template <typename T, size_t D> vec<int, D> logb(const vec<T, D>& x) {
  return ilobg(x);
}

// DEFINE_GEN_FUNC2(ldexp,  *);
template <typename T> T ldexp(const T& x, const int& y) { return x * (1 << y); }
template <typename T, size_t D>
vec<T, D> ldexp(const vec<T, D>& x, const int& y) {
  vec<int, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = ldexp(x[i], y);
  return R;
}

DEFINE_GEN_FUNC(lgamma, std::lgamma);
// DEFINE_GEN_FUNC2(lgamma_r,  *);
template <typename T, size_t D> T lgamma_r(const T& x, int* y) {
  vec<int, D> R;
  for (size_t i(0); i < D; i++) {
    R[i] = lgamma(x[i]);
    *y   = R[i] > 0 ? 0 : 1;
  }
  return R;
}

template <typename T, size_t D> T lgamma_r(const vec<T, D>& x, int* y) {
  vec<int, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = lgamma_r(x[i], &y[i]);
  return R;
}

DEFINE_GEN_FUNC(log, std::log);
DEFINE_GEN_FUNC(log2, std::log2);
DEFINE_GEN_FUNC(log10, std::log10);
DEFINE_GEN_FUNC(log1p, std::log1p);
DEFINE_GEN_FUNC(logb, std::logb);
DEFINE_GEN_FUNC3(mad, detail::mad);
DEFINE_GEN_FUNC2(maxmag, detail::maxmag);
DEFINE_GEN_FUNC2(minmag, detail::minmag);
// DEFINE_GEN_FUNC2(modf,  *);
template <typename T> T modf(const T& x, T* y) { return std::modf(x, y); }
template <typename T, size_t D> vec<T, D> modf(const vec<T, D>& x, T* y) {
  vec<T, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = modf(x[i], &y[i]);
  return R;
}

// DEFINE_GEN_FUNC_2T(uint, nan, std::nanf);
inline float nan(const uint& x) {
  return std::numeric_limits<float>::quiet_NaN();
}
template <size_t D> vec<float, D> nan(const vec<uint, D>& x) {
  vec<float, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = nan(x[i]);
  return R;
}
inline double nan(const ulonglong& x) {
  return std::numeric_limits<double>::quiet_NaN();
}
template <size_t D> vec<double, D> nan(const vec<ulonglong, D>& x) {
  vec<double, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = nan(x[i]);
  return R;
}

DEFINE_GEN_FUNC2(nextafter, std::nextafter);
DEFINE_GEN_FUNC2(pow, std::pow);
template <typename T> T pown(T x, int y) { return pow(x, T((float)y)); }
template <typename T, size_t D> vec<T, D> pown(const vec<T, D>& x, int y) {
  vec<T, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = pown(x[i], y);
}
DEFINE_GEN_FUNC2(powr, detail::powr);
DEFINE_GEN_FUNC(recip, detail::recip);
DEFINE_GEN_FUNC2(remainder, std::remainder);
template <typename T> T remquo(T x, T y, int* z) {
  return std::remquo(x, y, z);
}
template <typename T, size_t D>
vec<T, D> remquo(const vec<T, D>& x, const vec<T, D>& y, int* z) {
  vec<T, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = remquo(x[i], y[i], &z[i]);
  return R;
}
DEFINE_GEN_FUNC(rint, std::rint);

template <typename T> T rootn(T x, int y) { return detail::rootn(x, y); }
template <typename T, size_t D> vec<T, D> rootn(const vec<T, D>& x, int y) {
  vec<T, D> R;
  for (size_t i(0); i < D; i++)
    R[i] = rootn(x[i], y);
  return R;
}

DEFINE_GEN_FUNC(round, std::round);

DEFINE_GEN_FUNC(rsqrt, detail::rsqrt);

DEFINE_GEN_FUNC(sin, std::sin);

template <typename T> T sincos(T x, T* y) {
  *y = cos(x);
  return sin(x);
}
template <typename T, size_t D> vec<T, D> sincos(const vec<T, D>& x, T* y) {
  vec<T, D> R;
  for (size_t i(0); i < D; i++) {
    y[i] = cos(x[i]);
    R[i] = sin(x[i]);
  }
  return R;
}

DEFINE_GEN_FUNC(sinh, std::sinh);
DEFINE_GEN_FUNC(sinpi, detail::sinpi);
DEFINE_GEN_FUNC(sqrt, std::sqrt);
DEFINE_GEN_FUNC(tan, std::tan);
DEFINE_GEN_FUNC(tanh, std::tanh);
DEFINE_GEN_FUNC(tanpi, detail::tanpi);
DEFINE_GEN_FUNC(tgamma, std::tgamma);
DEFINE_GEN_FUNC(trunc, std::trunc);
} // namespace neosycl::sycl
