#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_OP_DEF_HPP
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_OP_DEF_HPP

#define DEFINE_OP_CONST(cls, op)                                                                     \
  friend cls<dimensions> operator op(const cls<dimensions> &lhs, const cls<dimensions> &rhs) {       \
    cls<dimensions> ret;                                                                             \
    for(std::size_t i = 0; i < dimensions; ++i) {                                                    \
      ret.data[i] = (size_t)(lhs._data[i] op rhs._data[i]);                                          \
    }                                                                                                \
    return ret;                                                                                      \
  };

#define DEFINE_OP_CONST_SIZE_T(cls, op)                                                              \
  friend cls<dimensions> operator op(const cls<dimensions> &lhs, const size_t &rhs) {                \
    cls<dimensions> ret;                                                                             \
    for(std::size_t i = 0; i < dimensions; ++i) {                                                    \
      ret.data[i] = (size_t)(lhs._data[i] op rhs);                                                   \
    }                                                                                                \
    return ret;                                                                                      \
  };

#define DEFINE_OP(cls, op)                                                                           \
  friend cls<dimensions> &operator op(cls<dimensions> &lhs, const cls<dimensions> &rhs) {            \
    for(std::size_t i = 0; i < dimensions; ++i) {                                                    \
      lhs.data[i] = (size_t)(lhs._data[i] op rhs._data[i]);                                          \
    }                                                                                                \
    return lhs;                                                                                      \
  };

#define DEFINE_OP_SIZE_T(cls, op)                                                                    \
  friend cls<dimensions> &operator op(cls<dimensions> &lhs, const size_t &rhs) {                     \
    for(std::size_t i = 0; i < dimensions; ++i) {                                                    \
      lhs.data[i] = (size_t)(lhs._data[i] op rhs);                                                   \
    }                                                                                                \
    return lhs;                                                                                      \
  };

#define DEFINE_OP_CONST_SIZE_T_LEFT(cls, op)                                                         \
  friend cls<dimensions> operator op(const size_t &lhs, cls<dimensions> &rhs) {                      \
    cls<dimensions> ret;                                                                             \
    for(std::size_t i = 0; i < dimensions; ++i) {                                                    \
      ret.data[i] = (size_t)(rhs._data[i] op lhs);                                                   \
    }                                                                                                \
    return ret;                                                                                      \
  };

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_OP_DEF_HPP
