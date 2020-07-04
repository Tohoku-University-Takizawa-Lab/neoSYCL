#ifndef SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_
#define SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_

#include "CL/SYCL/range.hpp"
#include "CL/SYCL/buffer/data_container.hpp"
#include "CL/SYCL/buffer/data_container_nd.hpp"

namespace cl::sycl::detail {

template<typename T>
class DataContainerND<T, 1> : public DataContainer {
 private:
  range<1> data_range;
  buffer_allocator<T> alloc;
  bool need_deallocate;
  T *data_ptr;

 private:
  void copy_from(const DataContainerND &obj) {
    if (obj.need_deallocate) {
      DEBUG_INFO("[DataContainer1D] allocate raw data with range: {}", obj.data_range.get(1));
      data_ptr = alloc.allocate(data_range.size());
      memcpy(data_ptr, obj.data_ptr, data_range.size() * sizeof(T));
    } else {
      data_ptr = obj.data_ptr;
    }
  }

  void deallocate() {
    if (this->need_deallocate) {
      DEBUG_INFO("[DataContainer1D] deallocate raw data with size: {}", data_range.size());
      this->alloc.deallocate(data_ptr, data_range.size());
    }
  }

 public:

  explicit DataContainerND(const cl::sycl::range<1> &r) : data_range(r), need_deallocate(true) {
    DEBUG_INFO("[DataContainer1D] create 1d container with range: {}, allocate size: {}",
               data_range.get(1),
               data_range.size());
    data_ptr = this->alloc.allocate(r.size());
  }

  explicit DataContainerND(T *data, const cl::sycl::range<1> &r) : data_range(r), need_deallocate(false) {
    DEBUG_INFO("[DataContainer1D] create 1d container with range {}, from raw data: {:#x}",
               data_range.get(1),
               (size_t) data);
    data_ptr = data;
  }

  template<typename Iterator>
  DataContainerND(Iterator start_iterator, Iterator end_iterator) :
      data_range(std::distance(start_iterator, end_iterator)),
      need_deallocate(false) {
    DEBUG_INFO("[DataContainer1D] copy construct 1d container with start iter: {:#x}, end iter: {:#x}, need_allocate: {}",
               (size_t) start_iterator,
               (size_t) end_iterator,
               false);
    data_ptr = start_iterator;
  }

  DataContainerND(const DataContainerND &obj) : data_range(obj.data_range), need_deallocate(obj.need_deallocate) {
    DEBUG_INFO("[DataContainer1D] copy construct 1d container with range {}, need_allocate: {}",
               obj.data_range.get(1),
               obj.need_deallocate);
    copy_from(obj);
  }

//  DataContainerND &operator=(const DataContainerND &obj) {
//    if (&obj != this) {
//      DEBUG_INFO("[DataContainer1D] operator copy construct 1d container with range {}, need_allocate: {}",
//                 obj.data_range.get(1),
//                 obj.need_deallocate);
//      deallocate();
//      data_range = obj.data_range;
//      need_deallocate = obj.need_deallocate;
//      copy_from();
//    }
//    return *this;
//  }

  DataContainerND &operator=(const DataContainerND &obj) = delete;

  size_t get_size() override {
    size_t ret = sizeof(T) * data_range.size();;
    return ret;
  }

  range<1> get_range() {
    return data_range;
  }

  void *get_data_ptr() override {
    return data_ptr;
  }

  T *begin() {
    return data_ptr;
  }

  T *end() {
    return data_ptr + data_range.size();
  }

  T &operator[](size_t x) const {
    return data_ptr[x];
  }

  T &get(size_t x) {
    return data_ptr[x];
  }

  T &get(const id<1> &index) {
    return data_ptr[index.get(1)];
  }

  virtual ~DataContainerND() {
    DEBUG_INFO("[DataContainer1D] deallocate 1d container with range {}, need_allocate: {}",
               data_range.get(1),
               need_deallocate);
    deallocate();
  }

};

}

#endif //SYCL_INCLUDE_CL_SYCL_BUFFER_DATA_CONTAINER_1D_HPP_
