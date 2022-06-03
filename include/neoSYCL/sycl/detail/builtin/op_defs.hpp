#pragma once

namespace neosycl::sycl {
/*
  N: the function name
  F: scalar function for each element of the output vector
*/
#define DEFINE_GEN_FUNC(N, F)                                                  \
  template <typename T>                                                        \
  T N(const T& i) {                                                            \
    return F(i);                                                               \
  }                                                                            \
  template <typename T, size_t D>                                              \
  vec<T, D> N(const vec<T, D>& x) {                                            \
    vec<T, D> R;                                                               \
    for (size_t i = 0; i < D; i++)                                             \
      R[i] = F(x[i]);                                                          \
    return R;                                                                  \
  }

#define DEFINE_UGEN_FUNC(N, F)                                                 \
  template <typename T>                                                        \
  typename std::make_unsigned<T>::type N(const T& i) {                         \
    return F(i);                                                               \
  }                                                                            \
  template <typename T, size_t D>                                              \
  vec<typename std::make_unsigned<T>::type, D> N(const vec<T, D>& x) {         \
    vec<typename std::make_unsigned<T>::type, D> R;                            \
    for (size_t i = 0; i < D; i++)                                             \
      R[i] = F(x[i]);                                                          \
    return R;                                                                  \
  }

#define DEFINE_GEN_FUNC2(N, F)                                                 \
  template <typename T>                                                        \
  T N(const T& x, const T& y) {                                                \
    return F(x, y);                                                            \
  }                                                                            \
  template <typename T, size_t D>                                              \
  vec<T, D> N(const vec<T, D>& x, const vec<T, D>& y) {                        \
    vec<T, D> R;                                                               \
    for (size_t i = 0; i < D; i++)                                             \
      R[i] = F(x[i], y[i]);                                                    \
    return R;                                                                  \
  }

#define DEFINE_UGEN_FUNC2(N, F)                                                \
  template <typename T>                                                        \
  typename std::make_unsigned<T>::type N(const T& x, const T& y) {             \
    return F(x, y);                                                            \
  }                                                                            \
  template <typename T, size_t D>                                              \
  vec<typename std::make_unsigned<T>::type, D> N(const vec<T, D>& x,           \
                                                 const vec<T, D>& y) {         \
    vec<typename std::make_unsigned<T>::type, D> R;                            \
    for (size_t i = 0; i < D; i++)                                             \
      R[i] = F(x[i], y[i]);                                                    \
    return R;                                                                  \
  }

#define DEFINE_GEN_FUNC3(N, F)                                                 \
  template <typename T>                                                        \
  T N(const T& x, const T& y, const T& z) {                                    \
    return F(x, y, z);                                                         \
  }                                                                            \
  template <typename T, size_t D>                                              \
  vec<T, D> N(const vec<T, D>& x, const vec<T, D>& y, const vec<T, D>& z) {    \
    vec<T, D> R;                                                               \
    for (size_t i = 0; i < D; i++)                                             \
      R[i] = F(x[i], y[i], z[i]);                                              \
    return R;                                                                  \
  }

/*
  OP: operator
  T: data type
  D: dimensions
*/
#define FRIEND_VEC_BINARY_OPERATOR(OP)                                         \
  template <typename T, size_t D>                                              \
  friend vec<T, D> operator OP(const vec<T, D>& lhs, const vec<T, D>& rhs);    \
  template <typename T, size_t D>                                              \
  friend vec<T, D> operator OP(const vec<T, D>& lhs, const T& rhs);            \
  template <typename T, size_t D>                                              \
  friend vec<T, D> operator OP(const T& lhs, const vec<T, D>& rhs);

#define DEF_VEC_BINARY_OPERATOR(OP)                                            \
  template <typename T, size_t D>                                              \
  vec<T, D> operator OP(const vec<T, D>& lhs, const vec<T, D>& rhs) {          \
    vec<T, D> r;                                                               \
    for (size_t i(0); i < D; i++) {                                            \
      r.data_[i] = lhs.data_[i] OP rhs.data_[i];                               \
    }                                                                          \
    return r;                                                                  \
  }                                                                            \
  template <typename T, size_t D>                                              \
  vec<T, D> operator OP(const vec<T, D>& lhs, const T& rhs) {                  \
    vec<T, D> r;                                                               \
    for (size_t i(0); i < D; i++) {                                            \
      r.data_[i] = lhs.data_[i] OP rhs;                                        \
    }                                                                          \
    return r;                                                                  \
  }                                                                            \
  template <typename T, size_t D>                                              \
  vec<T, D> operator OP(const T& lhs, const vec<T, D>& rhs) {                  \
    vec<T, D> r;                                                               \
    for (size_t i(0); i < D; i++) {                                            \
      r.data_[i] = rhs.data_[i] OP lhs;                                        \
    }                                                                          \
    return r;                                                                  \
  }

#define FRIEND_VEC_ASSIGN_OPERATOR(OP)                                         \
  template <typename T, size_t D>                                              \
  friend vec<T, D>& operator OP(vec<T, D>& lhs, const vec<T, D>& rhs);         \
  template <typename T, size_t D>                                              \
  friend vec<T, D>& operator OP(vec<T, D>& lhs, const T& rhs);

#define DEF_VEC_ASSIGN_OPERATOR(OP)                                            \
  template <typename T, size_t D>                                              \
  vec<T, D>& operator OP(vec<T, D>& lhs, const vec<T, D>& rhs) {               \
    for (size_t i(0); i < D; i++) {                                            \
      lhs.data_[i] OP rhs.data_[i];                                            \
    }                                                                          \
    return lhs;                                                                \
  }                                                                            \
  template <typename T, size_t D>                                              \
  vec<T, D>& operator OP(vec<T, D>& lhs, const T& rhs) {                       \
    vec<T, D> r;                                                               \
    for (size_t i(0); i < D; i++) {                                            \
      lhs.data_[i] OP rhs;                                                     \
    }                                                                          \
    return lhs;                                                                \
  }

#define FRIEND_VEC_INCDEC_OPERATOR(OP)                                         \
  template <typename T, size_t D>                                              \
  friend vec<T, D>& operator OP(vec<T, D>& lhs);                               \
  template <typename T, size_t D>                                              \
  friend vec<T, D> operator OP(vec<T, D>& lhs, int inc);

#define DEF_VEC_INCDEC_OPERATOR(OP)                                            \
  template <typename T, size_t D>                                              \
  vec<T, D>& operator OP(vec<T, D>& lhs) {                                     \
    for (size_t i(0); i < D; i++) {                                            \
      OP(lhs.data_[i]);                                                        \
    }                                                                          \
    return lhs;                                                                \
  }                                                                            \
  template <typename T, size_t D>                                              \
  vec<T, D> operator OP(vec<T, D>& lhs, int inc) {                             \
    vec<T, D> r;                                                               \
    for (size_t i(0); i < D; i++) {                                            \
      r[i] = (lhs.data_[i]);                                                   \
      (lhs.daha_[i]) OP;                                                       \
    }                                                                          \
    return r;                                                                  \
  }

#define FRIEND_VEC_BOOLEAN_OPERATOR(OP)                                        \
  template <typename RET, typename T, size_t D>                                \
  friend vec<RET, D> operator OP(const vec<T, D>& lhs, const vec<T, D>& rhs);  \
  template <typename RET, typename T, size_t D>                                \
  friend vec<RET, D> operator OP(const vec<T, D>& lhs, const T& rhs);          \
  template <typename RET, typename T, size_t D>                                \
  friend vec<RET, D> operator OP(const T& lhs, const vec<T, D>& rhs);

#define DEF_VEC_BOOLEAN_OPERATOR(OP)                                           \
  template <typename RET, typename T, size_t D>                                \
  vec<RET, D> operator OP(const vec<T, D>& lhs, const vec<T, D>& rhs) {        \
    vec<RET, D> r;                                                             \
    for (size_t i(0); i < D; i++)                                              \
      r.data_[i] = RET(lhs.data_[i] OP rhs.data_[i]);                          \
    return r;                                                                  \
  }                                                                            \
  template <typename RET, typename T, size_t D>                                \
  vec<RET, D> operator OP(const vec<T, D>& lhs, const T& rhs) {                \
    vec<RET, D> r;                                                             \
    for (size_t i(0); i < D; i++)                                              \
      r.data_[i] = RET(lhs.data_[i] OP rhs);                                   \
    return r;                                                                  \
  }                                                                            \
  template <typename RET, typename T, size_t D>                                \
  vec<RET, D> operator OP(const T& lhs, const vec<T, D>& rhs) {                \
    vec<RET, D> r;                                                             \
    for (size_t i(0); i < D; i++)                                              \
      r.data_[i] = RET(lhs OP rhs.data_[i]);                                   \
    return r;                                                                  \
  }

#define FRIEND_VEC_UNARY_OPERATOR(OP)                                          \
  template <typename T, size_t D>                                              \
  friend vec<T, D> operator OP(const vec<T, D>& v);

#define DEF_VEC_UNARY_OPERATOR(OP)                                             \
  template <typename T, size_t D>                                              \
  vec<T, D> operator OP(const vec<T, D>& v) {                                  \
    vec<T, D> r;                                                               \
    for (size_t i(0); i < D; i++) {                                            \
      r.data_[i] = operator OP(v.data_[i]);                                    \
    }                                                                          \
    return r;                                                                  \
  }
} // namespace neosycl::sycl
