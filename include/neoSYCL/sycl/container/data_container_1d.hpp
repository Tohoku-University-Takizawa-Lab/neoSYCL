#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_

#include <vector>
#include "data_container.hpp"

namespace neosycl::sycl::detail {

template<typename T, typename AllocatorT>
class DataContainerND<T, 1, AllocatorT> : public DataContainer {
 private:
  size_t dim0;
  std::shared_ptr<std::vector<T> > data_ptr;
  buffer_allocator<T> alloc;
  T *raw;

 public:
  explicit DataContainerND(size_t dim0) : dim0(dim0), data_ptr(alloc.allocate(dim0)), raw(data_ptr.get()->begin()) {}

  explicit DataContainerND(T *data, size_t dim0) : dim0(dim0), raw(data) {}

  size_t get_size() override {
    size_t ret = sizeof(T) * dim0;;
    return ret;
  }

  void *get_data_ptr() override {
    return (void *) raw;
  }

  T *begin() {
    return raw;
  }

  T *end() {
    return raw + dim0;
  }

  T &operator[](size_t x) const {
    return raw[x];
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_
