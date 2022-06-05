#pragma once
#include <map>
#include "neoSYCL/sycl/access.hpp"

namespace neosycl::sycl {

namespace detail {
class program_data;
};

namespace detail::container {

template <typename T, int dimensions, typename AllocatorT = buffer_allocator<T>>
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

  struct device_ptr {
    void* ptr;
    access::mode mode;
  };

  /* {program_data*, device_ptr} */
  std::map<shared_ptr_class<program_data>, device_ptr> map;
};
} // namespace detail::container
} // namespace neosycl::sycl
