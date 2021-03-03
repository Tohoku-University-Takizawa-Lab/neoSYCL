#ifndef SYCL_INCLUDE_CL_SYCL_ITEM_HPP_
#define SYCL_INCLUDE_CL_SYCL_ITEM_HPP_

namespace neosycl::sycl {

template<std::size_t dimensions>
struct id;

template<int dimensions = 1, bool with_offset = true>
struct item {
public:

  item() = delete;

  template<int D = dimensions, typename = std::enable_if_t<D == 1>>
  item(const range <dimensions> &r,
       const detail::ArrayND <dimensions> &index,
       const detail::ArrayND <dimensions> &offsets)
      : max_range(r), data{index}, offset{offsets} {
  }

  id<dimensions> get_id() const {
    return id<dimensions>(this);
  };

  size_t get_id(int dimension) const {
    return this->index[dimension];
  };

  size_t operator[](int dimension) const {
    return this->index[dimension];
  }

  range <dimensions> get_range() const {
    return this->max_range;
  }

  // only available if with_offset is true
  template<bool OFFSET = with_offset, typename = std::enable_if_t<OFFSET>>
  id<dimensions> get_offset() const {
    return this->get_id();
  }

  // only available if with_offset is false
  template<bool OFFSET = with_offset, typename = std::enable_if_t<!OFFSET>>
  operator item<dimensions, true>() const {
    return item<dimensions, true>(this->max_range, this->data, this->data);
  }

private:
  range <dimensions> max_range;
  detail::ArrayND <dimensions> offset;
  detail::ArrayND <dimensions> data;

};

}

#endif //SYCL_INCLUDE_CL_SYCL_ITEM_HPP_
