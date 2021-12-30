#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_OP_DEF_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_OP_DEF_HPP

#define DEFINE_OP_CONST(cls, op)                                               \
  friend cls<dimensions> operator op(const cls<dimensions> &lhs,               \
                                     const cls<dimensions> &rhs) {             \
    cls<dimensions> ret;                                                       \
    ret.data = lhs.data op rhs.data;                                           \
    return ret;                                                                \
  };

#define DEFINE_OP_CONST_SIZE_T(cls, op)                                        \
  friend cls<dimensions> operator op(const cls<dimensions> &lhs,               \
                                     const size_t &rhs) {                      \
    cls<dimensions> ret;                                                       \
    ret.data = lhs.data op rhs;                                                \
    return ret;                                                                \
  };

#define DEFINE_OP(cls, op)                                                     \
  friend cls<dimensions> &operator op(cls<dimensions> &lhs,                    \
                                      const cls<dimensions> &rhs) {            \
    lhs.data = lhs.data op rhs.data;                                           \
    return lhs;                                                                \
  };

#define DEFINE_OP_SIZE_T(cls, op)                                              \
  friend cls<dimensions> &operator op(cls<dimensions> &lhs,                    \
                                      const size_t &rhs) {                     \
    lhs.data = lhs.data op rhs;                                                \
    return lhs;                                                                \
  };

#define DEFINE_OP_CONST_SIZE_T_LEFT(cls, op)                                   \
  friend cls<dimensions> operator op(const size_t &lhs,                        \
                                     cls<dimensions> &rhs) {                   \
    cls<dimensions> ret;                                                       \
    ret.data = rhs.data op lhs;                                                \
    return ret;                                                                \
  };

#define DEFINE_COMMON_BY_VALUE_OP(cls, op)                                     \
  friend bool operator op(const cls<dimensions> &lhs,                          \
                          const cls<dimensions> &rhs) {                        \
    return lhs.data op rhs.data;                                               \
  }

#define DEFINE_COMMON_BY_VALUE_SEMANTICS(cls)                                  \
  DEFINE_COMMON_BY_VALUE_OP(cls, ==);                                          \
  DEFINE_COMMON_BY_VALUE_OP(cls, !=)

#endif // NEOSYCL_INCLUDE_NEOSYCL_SYCL_OP_DEF_HPP
