#ifndef SYCL_INCLUDE_CL_SYCL_ITEM_HPP_
#define SYCL_INCLUDE_CL_SYCL_ITEM_HPP_

namespace neosycl::sycl {

template<std::size_t dimensions>
struct id;

template<int dimensions = 1, bool with_offset = true>
struct item {
 public:

  item() = delete;

  id<dimensions> get_id() const {
    return id<dimensions>(this);
  };

  size_t get_id(int dimension) const {
    return this->index[dimension];
  };

  size_t operator[](int dimension) const {
    return this->index[dimension];
  }

  template<typename = std::enable_if_t<dimensions == 1>>
  range<dimensions> get_range() const {
    return range<dimensions>(this->size[0]);
  }

 private:

  size_t offset;
  size_t index[3];
  size_t size[3];

};

template<bool with_offset>
struct item<1, with_offset> {
  item() = delete;

  id<1> get_id() const;

  size_t get_id(int dimension) const;

  size_t operator[](int dimension) const;

  range<1> get_range() const;

  size_t get_range(int dimension) const;

  size_t get_linear_id() const;

  operator item<1, true>() const;

  id<1> get_offset() const;
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

  operator item<2, true>() const;

  id<2> get_offset() const;
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

  operator item<3, true>() const;

  id<3> get_offset() const;
};

}

#endif //SYCL_INCLUDE_CL_SYCL_ITEM_HPP_
