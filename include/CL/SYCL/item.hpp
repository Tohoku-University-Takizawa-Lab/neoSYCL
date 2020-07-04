//
// Created by WhiteBlue on 2020/5/26.
//

#ifndef SYCL_INCLUDE_CL_SYCL_ITEM_HPP_
#define SYCL_INCLUDE_CL_SYCL_ITEM_HPP_

namespace cl::sycl {
template<int dimensions>
struct item {
  item() = delete;
  item(const item &rhs) = default;

  id<dimensions> get() const;

  size_t get(int dimension) const;

  size_t &operator[](int dimension);

  range<dimensions> get_range() const;

  id<dimensions> get_offset() const;

  size_t get_linear_id() const;

  operator id<dimensions>();
}

#endif //SYCL_INCLUDE_CL_SYCL_ITEM_HPP_
