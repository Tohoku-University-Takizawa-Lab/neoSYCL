#pragma once
#include <map>
#include "neoSYCL/sycl/access.hpp"

namespace neosycl::sycl {
// class device;

namespace detail::container {

struct device_ptr {
  void* ptr;
  access::mode mode;
};

template <typename T, size_t dimensions,
          typename AllocatorT = buffer_allocator<T>>
class BufferContainer : public DataContainerND<T, dimensions, AllocatorT> {
public:
  BufferContainer(const ArrayND<dimensions>& r)
      : DataContainerND<T, dimensions, AllocatorT>(r) {}

  BufferContainer(const ArrayND<dimensions>& r, AllocatorT alloc)
      : DataContainerND<T, dimensions, AllocatorT>(r, alloc) {}

  BufferContainer(T* data, const ArrayND<dimensions>& r)
      : DataContainerND<T, dimensions, AllocatorT>(data, r) {}

  BufferContainer(T* data, const ArrayND<dimensions>& r, AllocatorT alloc)
      : DataContainerND<T, dimensions, AllocatorT>(data, r, alloc) {}

  BufferContainer(const DataContainerD<T, dimensions, AllocatorT>& rhs)
      : DataContainerND<T, dimensions, AllocatorT>(rhs) {}

  BufferContainer(DataContainerD<T, dimensions, AllocatorT>&& rhs)
      : DataContainerND<T, dimensions, AllocatorT>(rhs) {}

  ~BufferContainer();
  std::map<uint64_t, device_ptr> map;
};
} // namespace detail::container
} // namespace neosycl::sycl
