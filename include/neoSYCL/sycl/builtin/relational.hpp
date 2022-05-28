#pragma once

namespace neosycl::sycl {
#include <cstdint>
#include <cmath>

#define DEFINE_REL_FUNC(F, Exp)                                                \
  inline std::uint32_t F(float p0, float p1) { return std::uint32_t(Exp); }    \
  inline std::uint64_t F(double p0, double p1) { return std::uint64_t(Exp); }  \
  template <size_t D>                                                          \
  vec<std::uint32_t, D> F(const vec<float, D>& p0, const vec<float, D>& p1) {  \
    vec<std::uint32_t, D> r;                                                   \
    for (size_t i(0); i < D; i++)                                              \
      r[i] = F(p0[i], p1[i]);                                                  \
    return r;                                                                  \
  }                                                                            \
  template <size_t D>                                                          \
  vec<std::uint64_t, D> F(const vec<double, D>& p0,                            \
                          const vec<double, D>& p1) {                          \
    vec<std::uint64_t, D> r;                                                   \
    for (size_t i(0); i < D; i++)                                              \
      r[i] = F(p0[i], p1[i]);                                                  \
    return r;                                                                  \
  }

#define DEFINE_TEST_FUNC(F, Exp)                                               \
  inline std::uint32_t F(float p) { return std::uint32_t(Exp); }               \
  inline std::uint64_t F(double p) { return std::uint64_t(Exp); }              \
  template <size_t D> vec<std::uint32_t, D> F(const vec<float, D>& p) {        \
    vec<std::uint32_t, D> r;                                                   \
    for (size_t i(0); i < D; i++)                                              \
      r[i] = F(p[i]);                                                          \
    return r;                                                                  \
  }                                                                            \
  template <size_t D> vec<std::uint64_t, D> F(const vec<double, D>& p) {       \
    vec<std::uint64_t, D> r;                                                   \
    for (size_t i(0); i < D; i++)                                              \
      r[i] = F(p[i]);                                                          \
    return r;                                                                  \
  }

DEFINE_REL_FUNC(isequal, p0 == p1);
DEFINE_REL_FUNC(isnoequal, p0 != p1);
DEFINE_REL_FUNC(isgreater, p0 > p1);
DEFINE_REL_FUNC(isgreaterequal, p0 >= p1);
DEFINE_REL_FUNC(isless, p0 < p1);
DEFINE_REL_FUNC(islessequal, p0 <= p1);
DEFINE_REL_FUNC(islessgreater, (p0 < p1) || (p0 > p1));
DEFINE_TEST_FUNC(isfinite, std::isfinite(p));
DEFINE_TEST_FUNC(isinf, std::isinf(p));
DEFINE_TEST_FUNC(isnan, std::isnan(p));
DEFINE_TEST_FUNC(isnormal, std::isnormal(p));
DEFINE_REL_FUNC(isordered, isequal(p0, p0) && isequal(p1, p1));
DEFINE_REL_FUNC(isunordered, isnan(p0) || isnan(p1));
DEFINE_TEST_FUNC(signbit, std::signbit(p));

} // namespace neosycl::sycl
