#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_ND_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_ND_HPP_

#include <shared_mutex>
#include "array_nd.hpp"

namespace neosycl::sycl::detail::container {

template<typename T, typename AllocatorT = buffer_allocator <T>>
class DataContainerND : public DataContainer {
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
    memcpy(ptr, data, sizeof(T) * dim0);
  }

  DataContainerND(const DataContainerND &obj) :
      dim0(obj.dim0),
      alloc(obj.alloc) {
    allocate_ptr = shared_ptr_class<T>(alloc.allocate(dim0));
    ptr = allocate_ptr.get();
    memcpy(ptr, obj.ptr, sizeof(T) * dim0);
  }

  DataContainerND &operator=(const DataContainerND &obj) = delete;

  size_t get_size() override {
    return sizeof(T) * dim0;
  }

  T *get_ptr() {
    return ptr;
  }

  void *get_raw_ptr() override {
    return (void *) get_ptr();
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

  template<int dimensions>
  T &operator[](ArrayND <dimensions> item) const {
    return ptr[item.get_liner()];
  }

private:
  size_t dim0;
  AllocatorT alloc;
  T *ptr;
  shared_ptr_class <T> allocate_ptr;
};

}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_ND_HPP_
