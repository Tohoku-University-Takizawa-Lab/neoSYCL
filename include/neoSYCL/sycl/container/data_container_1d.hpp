#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_

#include "neoSYCL/sycl/range.hpp"
#include "data_container.hpp"

namespace neosycl::sycl::detail {

template<typename T, typename AllocatorT>
class DataContainerND<T, 1, AllocatorT> : public DataContainer {
 private:
  size_t dim0;
  buffer_allocator<T> alloc;
  bool need_deallocate;
  T *data_ptr;

 private:
  void copy_from(const DataContainerND &obj) {
    if (obj.need_deallocate) {
      data_ptr = alloc.allocate(dim0);
      memcpy(data_ptr, obj.data_ptr, dim0 * sizeof(T));
    } else {
      data_ptr = obj.data_ptr;
    }
  }

  void deallocate() {
    if (this->need_deallocate) {
      this->alloc.deallocate(data_ptr, dim0);
    }
  }

 public:
  DataContainerND(const DataContainerND &obj) = delete;

  DataContainerND &operator=(const DataContainerND &p) = delete;

  explicit DataContainerND(size_t dim0) : dim0(dim0), need_deallocate(true) {
    data_ptr = alloc.allocate(dim0);
  }

  explicit DataContainerND(T *data, size_t dim0) : dim0(dim0), need_deallocate(false) {
    data_ptr = data;
  }

  size_t get_size() override {
    size_t ret = sizeof(T) * dim0;;
    return ret;
  }

  void *get_data_ptr() override {
    return data_ptr;
  }

  T *begin() {
    return data_ptr;
  }

  T *end() {
    return data_ptr + dim0;
  }

  T &operator[](size_t x) const {
    return data_ptr[x];
  }

  T &get(size_t x) {
    return data_ptr[x];
  }

  virtual ~DataContainerND() {
    deallocate();
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_
