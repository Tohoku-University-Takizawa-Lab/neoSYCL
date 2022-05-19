#ifndef SYCL_INCLUDE_CL_SYCL_ITEM_HPP_
#define SYCL_INCLUDE_CL_SYCL_ITEM_HPP_

#include "neoSYCL/sycl/detail/container/array_nd.hpp"

#define DEFINE_ITEM_BY_VALUE_OP(cls)                                           \
  friend bool operator==(const cls<dimensions> &lhs,                           \
                         const cls<dimensions> &rhs) {                         \
    return (lhs.data == rhs.data) && (lhs.max_range == rhs.max_range) &&       \
           (lhs.offset == rhs.offset);                                         \
  }                                                                            \
  friend bool operator!=(const cls<dimensions> &lhs,                           \
                         const cls<dimensions> &rhs) {                         \
    return (lhs.data != rhs.data) || (lhs.max_range != rhs.max_range) ||       \
           (lhs.offset != rhs.offset);                                         \
  }

namespace neosycl::sycl {

template <std::size_t dimensions> struct id;

template <size_t dimensions = 1, bool with_offset = true> struct item {
  item() = delete;

  template <int D = dimensions, typename = std::enable_if_t<D == 1>>
  item(const range<dimensions> &r,
       const detail::container::ArrayND<dimensions> &index,
       const detail::container::ArrayND<dimensions> &offsets)
      : max_range(r), data{index}, offset{offsets} {}

  id<dimensions> get_id() const { return id<dimensions>(this); };

  size_t get_id(int dimension) const { return this->data[dimension]; };

  size_t operator[](int dimension) const { return this->data[dimension]; }

  range<dimensions> get_range() const { return this->max_range; }

  // only available if with_offset is true
  template <bool OFFSET = with_offset, typename = std::enable_if_t<OFFSET>>
  id<dimensions> get_offset() const {
    return this->get_id();
  }

  // only available if with_offset is false
  template <bool OFFSET = with_offset, typename = std::enable_if_t<!OFFSET>>
  operator item<dimensions, true>() const {
    return item<dimensions, true>(this->max_range, this->data, this->data);
  }

  DEFINE_ITEM_BY_VALUE_OP(item);

  range<dimensions> max_range;
  detail::container::ArrayND<dimensions> offset;
  detail::container::ArrayND<dimensions> data;
};

} // namespace neosycl::sycl

#endif // SYCL_INCLUDE_CL_SYCL_ITEM_HPP_
