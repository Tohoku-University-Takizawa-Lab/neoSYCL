#pragma once
#include <map>
#include "neoSYCL/sycl/access.hpp"

namespace neosycl::sycl {

namespace detail {
class program_data;
};

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

  /* keep the program object not to delete program_data before buffer
   * destruction */
  vector_class<program> progs; // See handler::alloc_mem_

  /* {program_data*, device_ptr} */
  std::map<program_data*, device_ptr> map; // See program_data::alloc_mem
};
} // namespace detail::container
} // namespace neosycl::sycl
