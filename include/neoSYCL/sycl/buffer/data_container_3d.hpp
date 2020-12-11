#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_3D_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_3D_HPP_

#include "neoSYCL/sycl/range.hpp"
#include "data_container.hpp"

namespace neosycl::sycl::detail {

template<typename T>
class DataContainerND<T, 3> : public DataContainer {
 private:
  range<3> data_range;
  buffer_allocator<T> alloc;
  buffer_allocator<T *> alloc_2d;
  buffer_allocator<T **> alloc_3d;

  bool need_deallocate;

  T *ptr_data;
  T ***ptr_3d;

  T ***build_access_ptr(T *data, size_t X, size_t Y, size_t Z) {
    DEBUG_INFO("[DataContainer3D] make access ptr with range-1d: {}, range-2d: {}, range-3d: {}",
               data_range.get(1),
               data_range.get(2),
               data_range.get(3));
    T ***access_ptr = alloc_3d.allocate(X);
    for (size_t x = 0; x < X; x++) {
      T **inner_ptr = alloc_2d.allocate(Y);
      for (size_t y = 0; y < Y; y++) {
        inner_ptr[y] = data + (X * y * Z);
      }
      access_ptr[x] = inner_ptr;
    }
    return access_ptr;
  }

  void deallocate() {
    DEBUG_INFO(
        "[DataContainer3D] deallocate 3d container with range-1d: {}, range-2d: {}, range-3d: {}, need_allocate: {}",
        data_range.get(1),
        data_range.get(2),
        data_range.get(3),
        need_deallocate);
    if (need_deallocate) {
      alloc.deallocate(ptr_data, data_range.size());
    }
    size_t X = data_range.get(1);
    size_t Y = data_range.get(2);
    // deallocate 2d ptr
    for (size_t i = 0; i < X; i++) {
      alloc_2d.deallocate(ptr_3d[i], Y);
    }
    // deallocate 3d ptr
    alloc_3d.deallocate(ptr_3d, X);
  }

 public:
  explicit DataContainerND(const range<3> &r) : data_range(r), need_deallocate(true) {
    DEBUG_INFO("[DataContainer3D] create 3d container with range-1d: {}, range-2d: {}, range-3d: {}, allocate size: {}",
               data_range.get(1),
               data_range.get(2),
               data_range.get(3),
               data_range.size());
    ptr_data = alloc.allocate(data_range.size());
    ptr_3d = build_access_ptr(ptr_data, data_range.get(1), data_range.get(2), data_range.get(3));
  }

  explicit DataContainerND(T *data, const range<3> &r) : data_range(r), need_deallocate(false) {
    DEBUG_INFO("[DataContainer3D] create 3d container with range-1d: {}, range-2d: {}, range-3d: {}, from raw data: {}",
               data_range.get(1),
               data_range.get(2),
               data_range.get(3),
               (size_t) data);

    ptr_data = data;
    ptr_3d = build_access_ptr(ptr_data, data_range.get(1), data_range.get(2), data_range.get(3));
  }

  DataContainerND(const DataContainerND &obj) : data_range(obj.data_range), need_deallocate(obj.need_deallocate) {
    DEBUG_INFO(
        "[DataContainer3D] copy construct 3d container with range-1d: {}, range-2d: {}, range-3d: {}, need_allocate: {}",
        obj.range.get(1),
        obj.range.get(2),
        obj.range.get(3),
        obj.need_deallocate);
    if (need_deallocate) {
      ptr_data = alloc.allocate(data_range.size());
      memcpy(ptr_data, obj.ptr_data, data_range.size() * sizeof(T));
    } else {
      ptr_data = obj.ptr_data;
    }
    ptr_3d = build_access_ptr(ptr_data);
  }

  DataContainerND &operator=(const DataContainerND &obj) {
    if (&obj != this) {
      DEBUG_INFO("[DataContainer2D] copy 3d container with range-1d: {}, range-2d: {}, range-3d: {}, need_allocate: {}",
                 obj.range.get(1),
                 obj.range.get(2),
                 obj.range.get(3),
                 obj.need_deallocate);
      deallocate();
      data_range = obj.data_range;
      need_deallocate = obj.need_deallocate;
      if (need_deallocate) {
        ptr_data = alloc.allocate(data_range.size());
        memcpy(ptr_data, obj.ptr_data, data_range.size() * sizeof(T));
      } else {
        ptr_data = obj.ptr_data;
      }
      ptr_3d = build_access_ptr(ptr_data);
    }
    return *this;
  }

  size_t get_size() override {
    size_t ret = sizeof(T) * data_range.size();
    return ret;
  }

  range<3> get_range() {
    return data_range;
  }

  void *get_data_ptr() override {
    return ptr_data;
  }

  T **operator[](size_t x) const {
    return ptr_3d[x];
  }

  T &get(size_t x, size_t y, size_t z) const {
    return ptr_3d[x][y][z];
  }

  T &get(const id<3> &index) const {
    return ptr_3d[index.get(1)][index.get(2)][index.get(3)];
  }

  T **get(size_t i) {
    return ptr_3d[i];
  }

  virtual ~DataContainerND() {
    deallocate();
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_3D_HPP_
