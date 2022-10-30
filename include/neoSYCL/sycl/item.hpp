#pragma once
#include "neoSYCL/sycl/detail/container/array_nd.hpp"

#define DEFINE_ITEM_BY_VALUE_OP(cls)                                           \
  friend bool operator==(const cls<dimensions>& lhs,                           \
                         const cls<dimensions>& rhs) {                         \
    return (lhs.data == rhs.data) && (lhs.max_range == rhs.max_range) &&       \
           (lhs.offset == rhs.offset);                                         \
  }                                                                            \
  friend bool operator!=(const cls<dimensions>& lhs,                           \
                         const cls<dimensions>& rhs) {                         \
    return (lhs.data != rhs.data) || (lhs.max_range != rhs.max_range) ||       \
           (lhs.offset != rhs.offset);                                         \
  }

namespace neosycl::sycl {

template <int dimensions>
struct id;

template <int dimensions = 1, bool with_offset = true>
struct item {
  item() = delete;

  template <int D = dimensions>
  item(const range<dimensions>& r,
       const detail::container::ArrayND<dimensions>& index,
       const detail::container::ArrayND<dimensions>& offsets)
      : max_range(r), data{index}, offset{offsets} {}

  id<dimensions> get_id() const {
    return id<dimensions>(*this);
  };

  size_t get_id(int dimension) const {
    return this->data[dimension];
  };

  size_t operator[](int dimension) const {
    return this->data[dimension];
  }

  range<dimensions> get_range() const {
    return this->max_range;
  }

  size_t get_range(int dimension) const {
    return this->max_range[dimension];
  }

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

  size_t get_linear_id() const {
    size_t idx = 0;
    if (dimensions > 0)
      idx = data[0];
    if (dimensions > 1)
      idx = idx * max_range[0] + data[1];
    if (dimensions > 2)
      idx = idx * max_range[1] + data[2];
    return idx;
  }

  DEFINE_ITEM_BY_VALUE_OP(item);

  range<dimensions> max_range;
  detail::container::ArrayND<dimensions> data;
  detail::container::ArrayND<dimensions> offset;
};

// experimental impl just for testing
template <int dimensions = 1, bool with_offset = true>
struct nd_item : public neosycl::sycl::item<dimensions, with_offset> {
  size_t get_global_linear_id() {
    return 0;
  }
};

} // namespace neosycl::sycl
