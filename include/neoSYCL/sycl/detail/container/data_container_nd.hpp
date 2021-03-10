#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_ND_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_ND_HPP_

#include <shared_mutex>
#include "array_nd.hpp"

namespace neosycl::sycl::detail::container {

template<typename T, size_t dimensions, typename AllocatorT>
class DataContainerD : public DataContainer {
public:
  explicit DataContainerD(ArrayND <dimensions> r) : size(r) {
    this->allocate_ptr = shared_ptr_class<T>(alloc.allocate(r.get_liner()));
    this->ptr = this->allocate_ptr.get();
  }

  DataContainerD(ArrayND <dimensions> r, AllocatorT allocatorT) : alloc(allocatorT), size(r) {
    this->allocate_ptr = shared_ptr_class<T>(alloc.allocate(r.get_liner()));
    this->ptr = this->allocate_ptr.get();
  }

  DataContainerD(T *data, ArrayND <dimensions> r) : size(r), ptr(data), allocate_ptr(nullptr) {}

  DataContainerD(T *data, ArrayND <dimensions> r, AllocatorT allocatorT) :
      ptr(data), alloc(allocatorT), size(r), allocate_ptr(nullptr) {}

  size_t get_size() override {
    return sizeof(T) * this->size.get_liner();
  }

  size_t get_count() override {
    return this->size.get_liner();
  }

  T *get_ptr() const {
    return this->ptr;
  }

  void *get_raw_ptr() override {
    return (void *) this->get_ptr();
  }

  T *begin() const {
    return this->ptr;
  }

  T *end() const {
    return this->ptr + this->size.get_liner();
  }

  T &get(size_t x) const {
    return this->ptr[x];
  }

  AllocatorT get_allocator() {
    return this->alloc;
  }

  ArrayND <dimensions> get_range() const {
    return this->size;
  }

  DataContainerD(const DataContainerD &rhs) :
      size(rhs.size),
      alloc(rhs.alloc) {
    this->allocate_ptr = shared_ptr_class<T>(alloc.allocate(size.get_liner()));
    this->ptr = this->allocate_ptr.get();
    memcpy(ptr, rhs.ptr, sizeof(T) * size.get_liner());
  }

  DataContainerD(DataContainerD &&rhs) :
      size(rhs.size),
      alloc(rhs.alloc),
      allocate_ptr(rhs.allocate_ptr),
      ptr(rhs.ptr) {}

  DataContainerD &operator=(const DataContainerD &rhs) {
    this->size = rhs.size;
    this->alloc = rhs.alloc;
    this->ptr = rhs.ptr;
    this->allocate_ptr = rhs.allocate_ptr;
  }

  DataContainerD &operator=(DataContainerD &&rhs) {
    this->size = rhs.size;
    this->alloc = rhs.alloc;
    this->ptr = rhs.ptr;
    this->allocate_ptr = rhs.allocate_ptr;
  }

private:
  ArrayND <dimensions> size;
  AllocatorT alloc;
  T *ptr;
  shared_ptr_class <T> allocate_ptr;
};

template<typename T, size_t dimensions, typename AllocatorT = buffer_allocator <T>>
class DataContainerND {};

template<typename T, typename AllocatorT>
class DataContainerND<T, 1, AllocatorT> : public DataContainerD<T, 1, AllocatorT> {
public:
  DataContainerND(const ArrayND<1> &r) : DataContainerD<T, 1, AllocatorT>(r) {}

  DataContainerND(const ArrayND<1> &r, AllocatorT alloc) :
      DataContainerD<T, 1, AllocatorT>(r, alloc) {}

  DataContainerND(T *data, const ArrayND<1> &r) :
      DataContainerD<T, 1, AllocatorT>(data, r) {}

  DataContainerND(T *data, const ArrayND<1> &r, AllocatorT alloc) :
      DataContainerD<T, 1, AllocatorT>(data, r, alloc) {}

  DataContainerND(const DataContainerD<T, 1, AllocatorT> &rhs) :
      DataContainerD<T, 1, AllocatorT>(rhs) {}

  DataContainerND(DataContainerD<T, 1, AllocatorT> &&rhs) :
      DataContainerD<T, 1, AllocatorT>(rhs) {}

  T &operator[](size_t x) const {
    return this->get_ptr()[x];
  }
};

template<typename T, typename AllocatorT>
class DataContainerND<T, 2, AllocatorT> : public DataContainerD<T, 2, AllocatorT> {
public:
  DataContainerND(const ArrayND<2> &r) : DataContainerD<T, 2, AllocatorT>(r) {}

  DataContainerND(const ArrayND<2> &r, AllocatorT alloc) :
      DataContainerD<T, 2, AllocatorT>(r, alloc) {}

  DataContainerND(T *data, const ArrayND<2> &r) :
      DataContainerD<T, 2, AllocatorT>(data, r) {}

  DataContainerND(T *data, const ArrayND<2> &r, AllocatorT alloc) :
      DataContainerD<T, 2, AllocatorT>(data, r, alloc) {}

  DataContainerND(const DataContainerD<T, 2, AllocatorT> &rhs) :
      DataContainerD<T, 2, AllocatorT>(rhs) {}

  DataContainerND(DataContainerD<T, 2, AllocatorT> &&rhs) :
      DataContainerD<T, 2, AllocatorT>(rhs) {}

  T *operator[](size_t x) const {
    size_t y = this->get_range()[1];
    return this->get_ptr() + (x * y);
  }
};

template<typename T, typename AllocatorT>
class DataContainerND<T, 3, AllocatorT> : public DataContainerD<T, 3, AllocatorT> {
public:
  DataContainerND(const ArrayND<3> &r) : DataContainerD<T, 3, AllocatorT>(r) {
    this->build_access_ptr();
  }

  DataContainerND(const ArrayND<3> &r, AllocatorT alloc) :
      DataContainerD<T, 3, AllocatorT>(r, alloc) {
    this->build_access_ptr();
  }

  DataContainerND(T *data, const ArrayND<3> &r) :
      DataContainerD<T, 3, AllocatorT>(data, r) {
    this->build_access_ptr();
  }

  DataContainerND(T *data, const ArrayND<3> &r, AllocatorT alloc) :
      DataContainerD<T, 3, AllocatorT>(data, r, alloc) {
    this->build_access_ptr();
  }

  DataContainerND(const DataContainerD<T, 3, AllocatorT> &rhs) :
      DataContainerD<T, 3, AllocatorT>(rhs) {}

  DataContainerND(DataContainerD<T, 3, AllocatorT> &&rhs) :
      DataContainerD<T, 3, AllocatorT>(rhs) {}

  T **operator[](size_t x) const {
    size_t y = this->get_range()[1];
    size_t z = this->get_range()[2];
    return this->access_ptr.get() + (x * y * z);
  }

private:
  shared_ptr_class<T *> access_ptr;

  void build_access_ptr() {
    size_t y = this->get_range()[1];
    size_t z = this->get_range()[2];
    this->access_ptr = shared_ptr_class<T *>(this->get_allocator().allocate(y));
    for (size_t i = 0; i < y; i++) {
      this->access_ptr.get()[i] = i * z;
    }
  }
};

}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_ND_HPP_
