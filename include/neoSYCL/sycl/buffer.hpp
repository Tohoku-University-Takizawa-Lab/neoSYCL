#ifndef CUSTOM_SYCL_INCLUDE_SYCL_BUFFER_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_BUFFER_HPP_

#include "types.hpp"
#include "range.hpp"
#include "access.hpp"
#include "accessor.hpp"
#include "allocator.hpp"
#include "handler.hpp"
#include "property_list.hpp"

#include "container/containers.hpp"
#include "detail/shared_ptr_implementation.hpp"

namespace neosycl::sycl {

template<typename T, int dimensions = 1, typename AllocatorT = buffer_allocator<T>>
class buffer : public detail::SharedPtrImplementation<detail::DataContainerND<T, dimensions>> {
public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using allocator_type = AllocatorT;

  buffer(const range<dimensions> &bufferRange, const property_list &propList = {})
      : bufferRange(bufferRange),
        detail::SharedPtrImplementation<detail::DataContainerND<T, dimensions, AllocatorT>>
            (new detail::DataContainerND<T, dimensions, AllocatorT>(bufferRange.size())) {
  }

  buffer(const range<dimensions> &bufferRange, AllocatorT allocator, const property_list &propList = {})
      : bufferRange(bufferRange),
        detail::SharedPtrImplementation<detail::DataContainerND<T, dimensions, AllocatorT>>
            (new detail::DataContainerND<T, dimensions, AllocatorT>(bufferRange.size())) {
  }

  buffer(T *hostData, const range<dimensions> &bufferRange, const property_list &propList = {}) :
      bufferRange(bufferRange),
      detail::SharedPtrImplementation<detail::DataContainerND<T, dimensions, AllocatorT>>
          (new detail::DataContainerND<T, dimensions, AllocatorT>(hostData, bufferRange.size())) {
  }

  buffer(T *hostData, const range<dimensions> &bufferRange, AllocatorT allocator, const property_list &propList = {}) :
      bufferRange(bufferRange),
      detail::SharedPtrImplementation<detail::DataContainerND<T, dimensions, AllocatorT>>
          (new detail::DataContainerND<T, dimensions, AllocatorT>(hostData, bufferRange.size())) {
  }

  buffer(const T *hostData, const range<dimensions> &bufferRange, const property_list &propList = {}) :
      bufferRange(bufferRange),
      detail::SharedPtrImplementation<detail::DataContainerND<T, dimensions, AllocatorT>>
          (new detail::DataContainerND<T, dimensions, AllocatorT>(hostData, bufferRange.size())) {

  }

  buffer(const T *hostData,
         const range<dimensions> &bufferRange,
         AllocatorT allocator,
         const property_list &propList = {}) :
      bufferRange(bufferRange),
      detail::SharedPtrImplementation<detail::DataContainerND<T, dimensions>>
          (new detail::DataContainerND<T, dimensions, AllocatorT>(hostData, bufferRange.size())) {
  }

  buffer(const shared_ptr_class<T> &hostData,
         const range<dimensions> &bufferRange, AllocatorT allocator, const property_list &propList = {}) :
      bufferRange(bufferRange),
      detail::SharedPtrImplementation<detail::DataContainerND<T, dimensions>>
          (new detail::DataContainerND<T, dimensions, AllocatorT>(hostData, bufferRange.size())) {
  }

  buffer(const shared_ptr_class<T> &hostData, const range<dimensions> &bufferRange, const property_list &propList = {})
      : bufferRange(bufferRange),
        detail::SharedPtrImplementation<detail::DataContainerND<T, dimensions>>
            (new detail::DataContainerND<T, dimensions>(hostData, bufferRange.size())) {
  }

//  template<typename InputIterator, typename = std::enable_if_t<dimensions == 1>>
//  buffer<T, dimensions>(InputIterator first,
//                        InputIterator last,
//                        AllocatorT allocator,
//                        const property_list &propList = {}) :
//      bufferRange((last - first) / sizeof(T)),
//      detail::SharedPtrImplementation<detail::DataContainerND<T, dimensions>>
//          (new detail::DataContainerND<T, dimensions>(first, (last - first) / sizeof(T))) {
//  }

//  template<typename InputIterator>
//  buffer<T, dimensions>(InputIterator first, InputIterator last,
//                        const property_list &propList = {});

//  buffer(buffer<T, dimensions, AllocatorT> b, const id<dimensions> &baseIndex,
//         const range<dimensions> &subRange);

  /* Available only when: dimensions == 1. */
//  buffer(cl_mem clMemObject, const context &syclContext, event availableEvent = {});

/* -- common interface members -- */
/* -- property interface members -- */
  range<dimensions> get_range() const {
    return bufferRange;
  }

  size_t get_count() const {
    return bufferRange.size();
  }

  size_t get_size() const {
    return get_count() * sizeof(T);
  }

  AllocatorT get_allocator() const {
    return AllocatorT();
  }

  template<access::mode mode, access::target target = access::target::global_buffer>
  accessor<T, dimensions, mode, target> get_access(
      handler &commandGroupHandler);
  template<access::mode mode>
  accessor<T, dimensions, mode, access::target::host_buffer> get_access();

  template<access::mode mode, access::target target = access::target::global_buffer>
  accessor<T, dimensions, mode, target> get_access(
      handler &commandGroupHandler, range<dimensions> accessRange, id<dimensions> accessOffset = {});

  template<access::mode mode>
  accessor<T, dimensions, mode, access::target::host_buffer> get_access(
      range<dimensions> accessRange, id<dimensions> accessOffset = {});

  template<typename Destination = std::nullptr_t>
  void set_final_data(Destination finalData = nullptr);

  void set_write_back(bool flag = true);

  bool is_sub_buffer() const;

  template<typename ReinterpretT, int ReinterpretDim>
  buffer<ReinterpretT, ReinterpretDim, AllocatorT> reinterpret(range<ReinterpretDim> reinterpretRange) const;

private:
  range<dimensions> bufferRange;
};
}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_BUFFER_HPP_
