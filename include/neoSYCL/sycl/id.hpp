#pragma once
#include "neoSYCL/sycl/detail/container/array_nd.hpp"

namespace neosycl::sycl {

template <int dimensions = 1>
struct id {
  id() = default;

  template <int D = dimensions, typename = std::enable_if_t<D == 1>>
  id(size_t dim0) : data{dim0} {}

  template <int D = dimensions, typename = std::enable_if_t<D == 2>>
  id(size_t dim0, size_t dim1) : data{dim0, dim1} {}

  template <int D = dimensions, typename = std::enable_if_t<D == 3>>
  id(size_t dim0, size_t dim1, size_t dim2) : data{dim0, dim1, dim2} {}

  id(const range<dimensions>& range) {
    for (int i = 0; i < dimensions; i++) {
      this->data[i] = range.get(i);
    }
  }

  id(const item<dimensions>& item) {
    for (int i = 0; i < dimensions; i++) {
      this->data[i] = item[i];
    }
  }

  size_t get(int dimension) const {
    return data[dimension];
  }

  size_t& operator[](int dimension) {
    return data[dimension];
  }

  size_t operator[](int dimension) const {
    return data[dimension];
  }

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_OP_CONST(id, +);
  DEFINE_OP_CONST(id, -);
  DEFINE_OP_CONST(id, *);
  DEFINE_OP_CONST(id, /);
  DEFINE_OP_CONST(id, %);
  DEFINE_OP_CONST(id, <<);
  DEFINE_OP_CONST(id, >>);
  DEFINE_OP_CONST(id, &);
  DEFINE_OP_CONST(id, |);
  DEFINE_OP_CONST(id, ^);
  DEFINE_OP_CONST(id, &&);
  DEFINE_OP_CONST(id, ||);
  DEFINE_OP_CONST(id, <);
  DEFINE_OP_CONST(id, >);
  DEFINE_OP_CONST(id, <=);
  DEFINE_OP_CONST(id, >=);

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_OP_CONST_SIZE_T(id, +);
  DEFINE_OP_CONST_SIZE_T(id, -);
  DEFINE_OP_CONST_SIZE_T(id, *);
  DEFINE_OP_CONST_SIZE_T(id, /);
  DEFINE_OP_CONST_SIZE_T(id, %);
  DEFINE_OP_CONST_SIZE_T(id, <<);
  DEFINE_OP_CONST_SIZE_T(id, >>);
  DEFINE_OP_CONST_SIZE_T(id, &);
  DEFINE_OP_CONST_SIZE_T(id, |);
  DEFINE_OP_CONST_SIZE_T(id, ^);
  DEFINE_OP_CONST_SIZE_T(id, &&);
  DEFINE_OP_CONST_SIZE_T(id, ||);
  DEFINE_OP_CONST_SIZE_T(id, <);
  DEFINE_OP_CONST_SIZE_T(id, >);
  DEFINE_OP_CONST_SIZE_T(id, <=);
  DEFINE_OP_CONST_SIZE_T(id, >=);

  // Where OP is: +=, -=,*=, /=, %=, <<=, >>=, &=, |=, ˆ=.
  DEFINE_OP(id, +=);
  DEFINE_OP(id, -=);
  DEFINE_OP(id, *=);
  DEFINE_OP(id, /=);
  DEFINE_OP(id, %=);
  DEFINE_OP(id, <<=);
  DEFINE_OP(id, >>=);
  DEFINE_OP(id, &=);
  DEFINE_OP(id, |=);
  DEFINE_OP(id, ^=);

  // Where OP is: +=, -=,*=, /=, %=, <<=, >>=, &=, |=, ˆ=.
  DEFINE_OP_SIZE_T(id, +);
  DEFINE_OP_SIZE_T(id, -=);
  DEFINE_OP_SIZE_T(id, *=);
  DEFINE_OP_SIZE_T(id, /=);
  DEFINE_OP_SIZE_T(id, %=);
  DEFINE_OP_SIZE_T(id, <<=);
  DEFINE_OP_SIZE_T(id, >>=);
  DEFINE_OP_SIZE_T(id, &=);
  DEFINE_OP_SIZE_T(id, |=);
  DEFINE_OP_SIZE_T(id, ^=);

  // Where OP is: +, -, *, /, %, <<, >>, &, |, ˆ, &&, ||, <, >, <=, >=.
  DEFINE_OP_CONST_SIZE_T_LEFT(id, +);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, -);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, *);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, /);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, %);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, <<);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, >>);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, &);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, |);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, ^);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, &&);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, ||);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, <);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, >);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, <=);
  DEFINE_OP_CONST_SIZE_T_LEFT(id, >=);

  DEFINE_COMMON_BY_VALUE_SEMANTICS(id);

  detail::container::ArrayND<dimensions> data;
};

} // namespace neosycl::sycl
