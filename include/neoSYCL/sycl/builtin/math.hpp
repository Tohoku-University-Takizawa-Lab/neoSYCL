#pragma once
#include <cmath>
#include "vec.hpp"

namespace neosycl::sycl {

#define DEFINE_GENFLOAT_FUNC(T, n, f)                                          \
  inline T n(T i) { return f(i); }                                             \
  inline T##2 n(T##2 i) { return {f(i[0]), f(i[1])}; }                         \
  inline T##3 n(T##3 i) { return {f(i[0]), f(i[1]), f(i[2])}; }                \
  inline T##4 n(T##4 i) { return {f(i[0]), f(i[1]), f(i[2]), f(i[3])}; }

#define DEFINE_GENFLOAT_FUNC_2T(T1, T2, n, f)                                  \
  inline T1 n(T2 i) { return (T1)f(i); }                                       \
  inline T1##2 n(T2##2 i) { return {(T1)f(i[0]), (T1)f(i[1])}; }               \
  inline T1##3 n(T2##3 i) { return {(T1)f(i[0]), (T1)f(i[1]), (T1)f(i[2])}; }  \
  inline T1##4 n(T2##4 i) {                                                    \
    return {(T1)f(i[0]), (T1)f(i[1]), (T1)f(i[2]), (T1)f(i[3])};               \
  }

#define DEFINE_GENFLOAT_FUNC2(T, n, f)                                         \
  inline T n(T i, T j) { return f(i, j); }                                     \
  inline T##2 n(T##2 i, T##2 j) { return {f(i[0], j[0]), f(i[1], j[1])}; }     \
  inline T##3 n(T##3 i, T##3 j) {                                              \
    return {f(i[0], j[0]), f(i[1], j[1]), f(i[2], j[2])};                      \
  }                                                                            \
  inline T##4 n(T##4 i, T##4 j) {                                              \
    return {f(i[0], j[0]), f(i[1], j[1]), f(i[2], j[2]), f(i[3], j[3])};       \
  }

#define DEFINE_GENFLOAT_FUNC3(T, n, f)                                         \
  inline T n(T i, T j, T k) { return f(i, j, k); }                             \
  inline T##2 n(T##2 i, T##2 j, T##2 k) {                                      \
    return {f(i[0], j[0], k[0]), f(i[1], j[1], k[1])};                         \
  }                                                                            \
  inline T##3 n(T##3 i, T##3 j, T##3 k) {                                      \
    return {f(i[0], j[0], k[0]), f(i[1], j[1], k[1]), f(i[2], j[2], k[2])};    \
  }                                                                            \
  inline T##4 n(T##4 i, T##4 j, T##4 k) {                                      \
    return {f(i[0], j[0], k[0]), f(i[1], j[1], k[1]), f(i[2], j[2], k[2]),     \
            f(i[3], j[3], k[3])};                                              \
  }

namespace detail {
template <typename T> T acospi(T i) { return std::acos(i) / M_PI; }
template <typename T> T asinpi(T i) { return std::asin(i) / M_PI; }
template <typename T> T atanpi(T i) { return std::atan(i) / M_PI; }
template <typename T> T atan2pi(T i, T j) { return std::atan2(i, j) / M_PI; }
template <typename T> T cospi(T i) { return std::cos(M_PI * i); }
template <typename T> T divide(T i, T j) { return i / j; }
template <typename T> T exp10(T i) { return std::exp(i + std::log(10)); }
template <typename T> T mad(T i, T j, T k) { return i * j + k; }
template <typename T> T powr(T i, T j) { return std::pow(i, j); }
template <typename T> T recip(T i) { return 1 / i; }
template <typename T> T rsqrt(T i) { return std::sqrt(-i); }
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

DEFINE_GENFLOAT_FUNC(float, acos, std::acos);
DEFINE_GENFLOAT_FUNC(float, acosh, std::acoshf);
DEFINE_GENFLOAT_FUNC(float, acospi, detail::acospi);
DEFINE_GENFLOAT_FUNC(float, asin, std::asin);
DEFINE_GENFLOAT_FUNC(float, asinh, std::asinhf);
DEFINE_GENFLOAT_FUNC(float, asinpi, detail::asinpi);
DEFINE_GENFLOAT_FUNC(float, atan, std::atan);
DEFINE_GENFLOAT_FUNC2(float, atan2, std::atan2);
DEFINE_GENFLOAT_FUNC(float, atanh, std::atanhf);
DEFINE_GENFLOAT_FUNC(float, atanpi, detail::atanpi);
DEFINE_GENFLOAT_FUNC2(float, atan2pi, detail::atan2pi);
DEFINE_GENFLOAT_FUNC(float, cbrt, std::cbrtf);
DEFINE_GENFLOAT_FUNC(float, ceil, std::ceil);
DEFINE_GENFLOAT_FUNC2(float, copysign, std::copysignf);
DEFINE_GENFLOAT_FUNC(float, cos, std::cos);
DEFINE_GENFLOAT_FUNC(float, cosh, std::cosh);
DEFINE_GENFLOAT_FUNC(float, cospi, detail::cospi);
DEFINE_GENFLOAT_FUNC2(float, divide, detail::divide);
DEFINE_GENFLOAT_FUNC(float, erfc, std::erfcf);
DEFINE_GENFLOAT_FUNC(float, erf, std::erf);
DEFINE_GENFLOAT_FUNC(float, exp, std::exp);
DEFINE_GENFLOAT_FUNC(float, exp2, std::exp2f);
DEFINE_GENFLOAT_FUNC(float, exp10, detail::exp10);
DEFINE_GENFLOAT_FUNC(float, expm1, std::expm1f);
DEFINE_GENFLOAT_FUNC(float, fabs, std::fabs);
DEFINE_GENFLOAT_FUNC2(float, fdim, std::fdimf);
DEFINE_GENFLOAT_FUNC(float, floor, std::floor);
DEFINE_GENFLOAT_FUNC3(float, fma, std::fmaf);
DEFINE_GENFLOAT_FUNC2(float, fmax, std::fmaxf);
template <typename T> T fmax(T x, float y) { return fmaxf(x, T(y)); }
DEFINE_GENFLOAT_FUNC2(float, fmin, std::fminf);
template <typename T> T fmin(T x, float y) { return fminf(x, T(y)); }
DEFINE_GENFLOAT_FUNC2(float, fmod, std::fmod);
// DEFINE_GENFLOAT_FUNC2(float, fract,  *);
// DEFINE_GENFLOAT_FUNC2(double, frexp, *);
DEFINE_GENFLOAT_FUNC2(float, hypot, std::hypotf);
// DEFINE_GENFLOAT_FUNC_2T(int, float, logb, std::logbf);
// DEFINE_GENFLOAT_FUNC_2T(int, double, logb, std::logb);
// DEFINE_GENFLOAT_FUNC2(float, ilogb,  *);
// DEFINE_GENFLOAT_FUNC2(double, logb,  *);
// DEFINE_GENFLOAT_FUNC2(float, ldexp,  *);
// DEFINE_GENFLOAT_FUNC2(double, ldexp, *);
DEFINE_GENFLOAT_FUNC(float, lgamma, std::lgammaf);
// DEFINE_GENFLOAT_FUNC2(float, lgamma_r,  *);
DEFINE_GENFLOAT_FUNC(float, log, std::log);
DEFINE_GENFLOAT_FUNC(float, log2, std::log2f);
DEFINE_GENFLOAT_FUNC(float, log10, std::log10);
DEFINE_GENFLOAT_FUNC(float, log1p, std::log1pf);
DEFINE_GENFLOAT_FUNC(float, logb, std::logbf);
DEFINE_GENFLOAT_FUNC3(float, mad, detail::mad);
// DEFINE_GENFLOAT_FUNC2(float, maxmag, detail::maxmag);
// DEFINE_GENFLOAT_FUNC2(float, minmag, detail::minmag);
// DEFINE_GENFLOAT_FUNC2(float, modf,  *);
// DEFINE_GENFLOAT_FUNC_2T(float, uint, nan, std::nanf);
// DEFINE_GENFLOAT_FUNC_2T(double, ulonglong, nan, std::nan);
DEFINE_GENFLOAT_FUNC2(float, nextafter, std::nextafterf);
DEFINE_GENFLOAT_FUNC2(float, pow, std::pow);
template <typename T> T pown(T x, int y) { return pow(x, T((float)y)); }
DEFINE_GENFLOAT_FUNC2(float, powr, detail::powr);
DEFINE_GENFLOAT_FUNC(float, recip, detail::recip);

/**/
DEFINE_GENFLOAT_FUNC(float, sqrt, std::sqrt);
} // namespace neosycl::sycl
