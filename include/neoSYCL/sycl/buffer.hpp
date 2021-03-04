#ifndef CUSTOM_SYCL_INCLUDE_SYCL_BUFFER_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_BUFFER_HPP_

#include "types.hpp"
#include "range.hpp"
#include "access.hpp"
#include "accessor.hpp"
#include "allocator.hpp"
#include "handler.hpp"
#include "context.hpp"
#include "property_list.hpp"
#include "detail/container/data_container.hpp"
#include "detail/container/data_container_nd.hpp"

namespace neosycl::sycl {

namespace property {
namespace buffer {
class use_host_ptr {
public:
  use_host_ptr() = default;
};
class use_mutex {
public:
  use_mutex(mutex_class &mutexRef);
  mutex_class *get_mutex_ptr() const;
};
class context_bound {
public:
  context_bound(context boundContext);
  context get_context() const;
};
} // namespace buffer
} // namespace property




template<typename T, int dimensions = 1, typename AllocatorT = buffer_allocator<T>>
class buffer {
  friend accessor<T, dimensions, access::mode::read, access::target::global_buffer>;
  friend accessor<T, dimensions, access::mode::write, access::target::global_buffer>;
  friend accessor<T, dimensions, access::mode::read_write, access::target::global_buffer>;

public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  using allocator_type = AllocatorT;

  buffer(const range<dimensions> &bufferRange, const property_list &propList = {}) :
      buffer(bufferRange, allocator_type(), propList) {}

  buffer(const range<dimensions> &bufferRange, AllocatorT allocator, const property_list &propList = {})
      : bufferRange(bufferRange),
        data(new detail::container::DataContainerND<T, AllocatorT>(bufferRange.size(), allocator)) {}

  buffer(T *hostData, const range<dimensions> &bufferRange, const property_list &propList = {}) :
      buffer(hostData, bufferRange, allocator_type(), propList) {}

  buffer(T *hostData, const range<dimensions> &bufferRange, AllocatorT allocator, const property_list &propList = {}) :
      bufferRange(bufferRange),
      data(new detail::container::DataContainerND<T, AllocatorT>(hostData,
                                                                 bufferRange.size(),
                                                                 allocator)) {}

  buffer(const T *hostData, const range<dimensions> &bufferRange, const property_list &propList = {}) :
      buffer(hostData, bufferRange, allocator_type(), propList) {}

  buffer(const T *hostData,
         const range<dimensions> &bufferRange,
         AllocatorT allocator,
         const property_list &propList = {}) :
      bufferRange(bufferRange),
      data(new detail::container::DataContainerND<T, AllocatorT>(hostData, bufferRange.size())) {}

  buffer(const shared_ptr_class<T> &hostData,
         const range<dimensions> &bufferRange, AllocatorT allocator, const property_list &propList = {}) :
      bufferRange(bufferRange),
      data(new detail::container::DataContainerND<T, AllocatorT>(hostData, bufferRange.size())) {}

  buffer(const shared_ptr_class<T> &hostData, const range<dimensions> &bufferRange, const property_list &propList = {})
      : bufferRange(bufferRange),
        data(new detail::container::DataContainerND<T>(hostData, bufferRange.size())) {}

  template<typename InputIterator, int D = dimensions, typename = std::enable_if_t<D == 1>>
  buffer(InputIterator first,
         InputIterator last,
         AllocatorT allocator,
         const property_list &propList = {}) :
      bufferRange((last - first) / sizeof(T)),
      data(new detail::container::DataContainerND<T, AllocatorT>(first, (last - first) / sizeof(T))) {}

  template<typename InputIterator, int D = dimensions, typename = std::enable_if_t<D == 1>>
  buffer(InputIterator first, InputIterator last, const property_list &propList = {}) :
      bufferRange((last - first) / sizeof(T)),
      data(new detail::container::DataContainerND<T>(first, (last - first) / sizeof(T))) {}

  buffer(buffer<T, dimensions, AllocatorT> b, const id<dimensions> &baseIndex,
         const range<dimensions> &subRange) {}

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
  accessor<T, dimensions, mode, target> get_access(handler &commandGroupHandler) {

  }

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
  std::shared_ptr<detail::container::DataContainerND<T>> data;

  range<dimensions> bufferRange;
};
}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_BUFFER_HPP_
