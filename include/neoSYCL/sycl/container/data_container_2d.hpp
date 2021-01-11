#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_2D_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_2D_HPP_

#include "neoSYCL/sycl/range.hpp"
#include "data_container.hpp"

namespace neosycl::sycl::detail {

template<typename T, typename AllocatorT>
class DataContainerND<T, 2, AllocatorT> : public DataContainer {
 private:
  range<2> data_range;
  buffer_allocator<T> alloc;
  buffer_allocator<T *> ptr_alloc;
  bool need_deallocate;

  T *data_ptr;
  T **access_ptr;

  T **build_access_ptr(T *data) {
    DEBUG_INFO("[DataContainer2D] make access ptr with range-1d: {}, range-2d: {}",
               data_range.get(1),
               data_range.get(2));
    // do allocate inner ptr
    size_t x = data_range.get(1);
    size_t y = data_range.get(2);
    T **ptr = ptr_alloc.allocate(x);

    // do init location
    for (size_t i = 0; i < x; i++) {
      ptr[i] = data + (i * y);
    }

    return ptr;
  }

  void copy_from(const DataContainerND &obj) {
    if (obj.need_deallocate) {
      DEBUG_INFO("[DataContainer2D] allocate raw data with range-1d: {}, range-2d: {}",
                 obj.data_range.get(1),
                 obj.data_range.get(2));
      data_ptr = alloc.allocate(data_range.size());
      memcpy(data_ptr, obj.data_ptr, data_range.size() * sizeof(T));
    } else {
      data_ptr = obj.data_ptr;
    }
    access_ptr = build_access_ptr(data_ptr);
  }

  void deallocate() {
    DEBUG_INFO("[DataContainer2D] deallocate 2d container with range-1d: {}, range-2d: {}, need_allocate: {}",
               data_range.get(1),
               data_range.get(2),
               need_deallocate);
    if (need_deallocate) {
      alloc.deallocate(data_ptr, data_range.size());
    }
    size_t x = data_range.get(1);
    ptr_alloc.deallocate(access_ptr, x);
  }

 public:
  DataContainerND(DataContainerND &obj) = delete;

  explicit DataContainerND(const range<2> &r) : data_range(r), need_deallocate(true) {
    access_ptr = build_access_ptr(data_ptr);
  }

  explicit DataContainerND(T *data, const range<2> &r) : data_range(r), need_deallocate(false) {
    data_ptr = data;
    access_ptr = build_access_ptr(data_ptr);
  }

  DataContainerND(const DataContainerND &obj) : data_range(obj.data_range), need_deallocate(obj.need_deallocate) {
    copy_from(obj);
  }

  DataContainerND &operator=(const DataContainerND &obj) {
    if (&obj != this) {
      deallocate();
      data_range = obj.data_range;
      need_deallocate = obj.need_deallocate;
      copy_from();
    }
    return *this;
  }

  size_t get_size() override {
    size_t ret = sizeof(T) * data_range.size();
    return ret;
  }

  range<2> get_range() {
    return data_range;
  }

  void *get_data_ptr() override {
    return data_ptr;
  }

  T *operator[](size_t x) const {
    return this->access_ptr[x];
  }

  T &get(size_t dim0, size_t dim1) const {
    return access_ptr[dim0][dim1];
  }

  T *get(size_t dim0) {
    return access_ptr[dim0];
  }

  virtual ~DataContainerND() {
    deallocate();
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_2D_HPP_
