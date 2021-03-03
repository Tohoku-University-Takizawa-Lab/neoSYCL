#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_

#include <vector>
#include "data_container.hpp"

namespace neosycl::sycl::detail {

template<typename T, typename AllocatorT>
class DataContainerND<T, 1, AllocatorT> : public DataContainer {
private:
  size_t dim0;
  AllocatorT alloc;
  T *ptr;
  shared_ptr_class <T> allocate_ptr;

public:
  explicit DataContainerND(size_t dim0) : dim0(dim0) {
    allocate_ptr = shared_ptr_class<T>(alloc.allocate(dim0));
    ptr = allocate_ptr.get();
  }

  DataContainerND(size_t dim0, AllocatorT allocatorT) : alloc(allocatorT), dim0(dim0) {
    allocate_ptr = shared_ptr_class<T>(alloc.allocate(dim0));
    ptr = allocate_ptr.get();
  }

  DataContainerND(T *data, size_t dim0) : dim0(dim0), ptr(data) {}

  DataContainerND(const T *data, size_t dim0) : dim0(dim0) {
    allocate_ptr = shared_ptr_class<T>(alloc.allocate(dim0));
    ptr = allocate_ptr.get();
    memccpy(ptr, data, sizeof(T) * dim0);
  }

  DataContainerND(const T *data, size_t dim0, AllocatorT allocatorT) : alloc(allocatorT), dim0(dim0) {
    allocate_ptr = shared_ptr_class<T>(alloc.allocate(dim0));
    ptr = allocate_ptr.get();
    memccpy(ptr, data, sizeof(T) * dim0);
  }

  DataContainerND(const DataContainerND &obj) :
      dim0(obj.dim0),
      alloc(obj.alloc) {
    allocate_ptr = shared_ptr_class<T>(alloc.allocate(dim0));
    ptr = allocate_ptr.get();
    memcpy(ptr, obj.ptr, sizeof(T) * dim0);
  }

  DataContainerND &operator=(const DataContainerND &obj) = delete;

  size_t get_size() {
    return sizeof(T) * dim0;
  }

  T *get_ptr() {
    return ptr;
  }

  T *begin() {
    return ptr;
  }

  T *end() {
    return ptr + dim0;
  }

  T &operator[](size_t x) const {
    return ptr[x];
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_
