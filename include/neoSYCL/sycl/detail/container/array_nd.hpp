#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_BUFFER_ARRAY_ND_HPP_
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_BUFFER_ARRAY_ND_HPP_

#define DEFINE_ARRAY_ND_OP_CONST(op)                                           \
  friend ArrayND<dimensions> operator op(const ArrayND<dimensions>& lhs,       \
                                         const ArrayND<dimensions>& rhs) {     \
    ArrayND<dimensions> ret;                                                   \
    for (std::size_t i = 0; i < dimensions; ++i) {                             \
      ret[i] = (size_t)(lhs[i] op rhs[i]);                                     \
    }                                                                          \
    return ret;                                                                \
  };

#define DEFINE_ARRAY_ND_OP_CONST_SIZE_T(op)                                    \
  friend ArrayND<dimensions> operator op(const ArrayND<dimensions>& lhs,       \
                                         const size_t& rhs) {                  \
    ArrayND<dimensions> ret;                                                   \
    for (std::size_t i = 0; i < dimensions; ++i) {                             \
      ret[i] = (size_t)(lhs[i] op rhs);                                        \
    }                                                                          \
    return ret;                                                                \
  };

#define DEFINE_ARRAY_ND_OP(op)                                                 \
  friend ArrayND<dimensions>& operator op(ArrayND<dimensions>& lhs,            \
                                          const ArrayND<dimensions>& rhs) {    \
    for (std::size_t i = 0; i < dimensions; ++i) {                             \
      lhs[i] = (size_t)(lhs[i] op rhs[i]);                                     \
    }                                                                          \
    return lhs;                                                                \
  };

#define DEFINE_ARRAY_ND_OP_SIZE_T(op)                                          \
  friend ArrayND<dimensions>& operator op(ArrayND<dimensions>& lhs,            \
                                          const size_t& rhs) {                 \
    for (std::size_t i = 0; i < dimensions; ++i) {                             \
      lhs[i] = (size_t)(lhs[i] op rhs);                                        \
    }                                                                          \
    return lhs;                                                                \
  };

#define DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(op)                               \
  friend ArrayND<dimensions> operator op(const size_t& lhs,                    \
                                         ArrayND<dimensions>& rhs) {           \
    ArrayND<dimensions> ret;                                                   \
    for (std::size_t i = 0; i < dimensions; ++i) {                             \
      ret[i] = (size_t)(rhs[i] op lhs);                                        \
    }                                                                          \
    return ret;                                                                \
  };

#define DEFINE_ARRAY_ND_COMMON_BY_VALUE_SEMANTICS()                            \
  friend bool operator==(const ArrayND<dimensions>& lhs,                       \
                         const ArrayND<dimensions>& rhs) {                     \
    for (std::size_t i = 0; i < dimensions; ++i) {                             \
      if (lhs[i] != rhs[i]) {                                                  \
        return false;                                                          \
      }                                                                        \
    }                                                                          \
    return true;                                                               \
  }                                                                            \
  friend bool operator!=(const ArrayND<dimensions>& lhs,                       \
                         const ArrayND<dimensions>& rhs) {                     \
    for (std::size_t i = 0; i < dimensions; ++i) {                             \
      if (lhs[i] != rhs[i]) {                                                  \
        return true;                                                           \
      }                                                                        \
    }                                                                          \
    return false;                                                              \
  }

namespace neosycl::sycl::detail::container {

template <size_t dimensions = 1>
struct ArrayND {
  ArrayND() : data{} {}

  template <int D = dimensions, typename = std::enable_if_t<D == 1>>
  ArrayND(size_t dim0) : data{dim0} {}

  template <int D = dimensions, typename = std::enable_if_t<D == 2>>
  ArrayND(size_t dim0, size_t dim1) : data{dim0, dim1} {}

  template <int D = dimensions, typename = std::enable_if_t<D == 3>>
  ArrayND(size_t dim0, size_t dim1, size_t dim2) : data{dim0, dim1, dim2} {}

  size_t operator[](int dimension) const { return data[dimension]; }

  size_t& operator[](int dimension) { return data[dimension]; }

  size_t get_liner() const {
    if (dimensions == 2) {
      return data[0] * data[1];
    }
    else if (dimensions == 3) {
      return data[0] * data[1] * data[2];
    }
    return data[0];
  }

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_ARRAY_ND_OP_CONST(+);
  DEFINE_ARRAY_ND_OP_CONST(-);
  DEFINE_ARRAY_ND_OP_CONST(*);
  DEFINE_ARRAY_ND_OP_CONST(/);
  DEFINE_ARRAY_ND_OP_CONST(%);
  DEFINE_ARRAY_ND_OP_CONST(<<);
  DEFINE_ARRAY_ND_OP_CONST(>>);
  DEFINE_ARRAY_ND_OP_CONST(&);
  DEFINE_ARRAY_ND_OP_CONST(|);
  DEFINE_ARRAY_ND_OP_CONST(^);
  DEFINE_ARRAY_ND_OP_CONST(&&);
  DEFINE_ARRAY_ND_OP_CONST(||);
  DEFINE_ARRAY_ND_OP_CONST(<);
  DEFINE_ARRAY_ND_OP_CONST(>);
  DEFINE_ARRAY_ND_OP_CONST(<=);
  DEFINE_ARRAY_ND_OP_CONST(>=);

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(+);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(-);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(*);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(/);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(%);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(<<);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(>>);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(&);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(|);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(^);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(&&);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(||);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(<);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(>);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(<=);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T(>=);

  // Where OP is: +=, -=,*=, /=, %=, <<=, >>=, &=, |=, ˆ=.
  DEFINE_ARRAY_ND_OP(+=);
  DEFINE_ARRAY_ND_OP(-=);
  DEFINE_ARRAY_ND_OP(*=);
  DEFINE_ARRAY_ND_OP(/=);
  DEFINE_ARRAY_ND_OP(%=);
  DEFINE_ARRAY_ND_OP(<<=);
  DEFINE_ARRAY_ND_OP(>>=);
  DEFINE_ARRAY_ND_OP(&=);
  DEFINE_ARRAY_ND_OP(|=);
  DEFINE_ARRAY_ND_OP(^=);

  // Where OP is: +=, -=,*=, /=, %=, <<=, >>=, &=, |=, ˆ=.
  DEFINE_ARRAY_ND_OP_SIZE_T(+);
  DEFINE_ARRAY_ND_OP_SIZE_T(-=);
  DEFINE_ARRAY_ND_OP_SIZE_T(*=);
  DEFINE_ARRAY_ND_OP_SIZE_T(/=);
  DEFINE_ARRAY_ND_OP_SIZE_T(%=);
  DEFINE_ARRAY_ND_OP_SIZE_T(<<=);
  DEFINE_ARRAY_ND_OP_SIZE_T(>>=);
  DEFINE_ARRAY_ND_OP_SIZE_T(&=);
  DEFINE_ARRAY_ND_OP_SIZE_T(|=);
  DEFINE_ARRAY_ND_OP_SIZE_T(^=);

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(+);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(-);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(*);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(/);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(%);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(<<);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(>>);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(&);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(|);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(^);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(&&);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(||);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(<);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(>);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(<=);
  DEFINE_ARRAY_ND_OP_CONST_SIZE_T_LEFT(>=);

  DEFINE_ARRAY_ND_COMMON_BY_VALUE_SEMANTICS();

private:
  size_t data[dimensions];
};

} // namespace neosycl::sycl::detail::container

#endif // NEOSYCL_INCLUDE_NEOSYCL_SYCL_BUFFER_ARRAY_ND_HPP_
