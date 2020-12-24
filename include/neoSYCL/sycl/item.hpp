#ifndef SYCL_INCLUDE_CL_SYCL_ITEM_HPP_
#define SYCL_INCLUDE_CL_SYCL_ITEM_HPP_

#include "id.hpp"

namespace neosycl::sycl {

template<std::size_t dimensions>
struct id;

template<int dimensions = 1, bool with_offset = true>
struct item {};

template<bool with_offset>
struct item<1, with_offset> {
  item() = delete;

  id<1> get_id() const;

  size_t get_id(int dimension) const;

  size_t operator[](int dimension) const;

  range<1> get_range() const;

  size_t get_range(int dimension) const;

  size_t get_linear_id() const;
};

template<>
struct item<1, true> {
  id<1> get_offset() const;
};

template<>
struct item<1, false> {
  operator item<1, true>() const;
};

template<bool with_offset>
struct item<2, with_offset> {
  item() = delete;

  id<2> get_id() const;

  size_t get_id(int dimension) const;

  size_t operator[](int dimension) const;

  range<2> get_range() const;

  size_t get_range(int dimension) const;

  size_t get_linear_id() const;
};

template<>
struct item<2, true> {
  id<2> get_offset() const;
};

template<>
struct item<2, false> {
  operator item<2, true>() const;
};

template<bool with_offset>
struct item<3, with_offset> {
  item() = delete;

  id<3> get_id() const;

  size_t get_id(int dimension) const;

  size_t operator[](int dimension) const;

  range<3> get_range() const;

  size_t get_range(int dimension) const;

  size_t get_linear_id() const;
};

template<>
struct item<3, true> {
  id<3> get_offset() const;
};

template<>
struct item<3, false> {
  operator item<3, true>() const;
};

}

#endif //SYCL_INCLUDE_CL_SYCL_ITEM_HPP_
