#pragma once
#include <future>
#include "neoSYCL/sycl/detail/container/data_container.hpp"
#include "neoSYCL/sycl/detail/container/data_container_nd.hpp"
#include "neoSYCL/sycl/detail/futures.hpp"

namespace neosycl::sycl {

namespace property {
namespace buffer {
class use_host_ptr {
public:
  use_host_ptr() = default;
};
class use_mutex {
public:
  use_mutex(mutex_class& mutexRef);
  mutex_class* get_mutex_ptr() const;
};
class context_bound {
public:
  context_bound(context boundContext);
  context get_context() const;
};
} // namespace buffer
} // namespace property

///////////////////////////////////////////////////////////////////////////////
template <typename T, int dimensions = 1,
          typename AllocatorT = buffer_allocator<T>>
class buffer {
public:
  using value_type      = T;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using allocator_type  = AllocatorT;
  using container_type  = detail::container::BufferContainer<T, dimensions>;

  buffer(const buffer& rhs) = default;
  buffer(buffer&& rhs)      = default;
  ~buffer()                 = default;

  buffer& operator=(const buffer& rhs) = default;
  buffer& operator=(buffer&& rhs)      = default;

  template <typename Ty, int D, typename A>
  friend bool operator==(const buffer<Ty, D, A>& lhs,
                         const buffer<Ty, D, A>& rhs);
  template <typename Ty, int D, typename A>
  friend bool operator!=(const buffer<Ty, D, A>& lhs,
                         const buffer<Ty, D, A>& rhs);

  buffer(const range<dimensions>& bufferRange,
         const property_list& propList = {})
      : buffer(bufferRange, allocator_type(), propList) {}

  buffer(const range<dimensions>& bufferRange, AllocatorT allocator,
         const property_list& propList = {})
      : bufferRange(bufferRange),
        data(new container_type(bufferRange.data, allocator)) {}

  buffer(T* hostData, const range<dimensions>& bufferRange,
         const property_list& propList = {})
      : buffer(hostData, bufferRange, allocator_type(), propList) {}

  buffer(T* hostData, const range<dimensions>& bufferRange,
         AllocatorT allocator, const property_list& propList = {})
      : bufferRange(bufferRange),
        data(new container_type(hostData, bufferRange.data, allocator)) {}

  buffer(const T* hostData, const range<dimensions>& bufferRange,
         const property_list& propList = {})
      : buffer(hostData, bufferRange, allocator_type(), propList) {}

  buffer(const T* hostData, const range<dimensions>& bufferRange,
         AllocatorT allocator, const property_list& propList = {})
      : bufferRange(bufferRange),
        data(new container_type(hostData, bufferRange.data)) {}

  buffer(const shared_ptr_class<T>& hostData,
         const range<dimensions>& bufferRange, AllocatorT allocator,
         const property_list& propList = {})
      : bufferRange(bufferRange),
        data(new container_type(hostData, bufferRange.data, allocator)) {}

  buffer(const shared_ptr_class<T>& hostData,
         const range<dimensions>& bufferRange,
         const property_list& propList = {})
      : bufferRange(bufferRange),
        data(new container_type(hostData.get(), bufferRange.data)) {}

  template <typename InputIterator, int D = dimensions,
            typename = std::enable_if_t<D == 1>>
  buffer(InputIterator first, InputIterator last, AllocatorT allocator,
         const property_list& propList = {})
      : bufferRange((last - first) / sizeof(T)),
        data(new container_type(
            first, detail::container::ArrayND<1>((last - first) / sizeof(T)),
            allocator)) {}

  template <typename InputIterator, int D = dimensions,
            typename = std::enable_if_t<D == 1>>
  buffer(InputIterator first, InputIterator last,
         const property_list& propList = {})
      : bufferRange((last - first) / sizeof(T)),
        data(new container_type(
            first, detail::container::ArrayND<1>(last - first) / sizeof(T))) {}

  buffer(buffer<T, dimensions, AllocatorT> b, const id<dimensions>& baseIndex,
         const range<dimensions>& subRange);

  /* Available only when: dimensions == 1. */
  //  buffer(cl_mem clMemObject, const context &syclContext, event
  //  availableEvent = {});

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

  template <access::mode mode,
            access::target target = access::target::global_buffer>
  accessor<T, dimensions, mode, target>
  get_access(handler& commandGroupHandler) {
    if (commandGroupHandler.access_readonly()) {
      get_futures<mode>(commandGroupHandler);
      commandGroupHandler.GetBufferFutureInfo<mode>(&fs);
      accessor<T, dimensions, mode, target> acc(*this);
      return acc;
    }

    accessor<T, dimensions, mode, target> acc(*this);
    commandGroupHandler.alloc_mem_(acc);
    return acc;
  }

  template <access::mode mode>
  accessor<T, dimensions, mode, access::target::host_buffer> get_access() {
    return accessor<T, dimensions, mode, access::target::host_buffer>(*this);
  }

  template <access::mode mode,
            access::target target = access::target::global_buffer>
  accessor<T, dimensions, mode, target>
  get_access(handler& commandGroupHandler, range<dimensions> accessRange,
             id<dimensions> accessOffset = {}) {
    if (commandGroupHandler.access_readonly()) {
      get_futures<mode>(commandGroupHandler);
      commandGroupHandler.GetBufferFutureInfo<mode>(&fs);
      accessor<T, dimensions, mode, target> acc(*this);
      return acc;
    }
    accessor<T, dimensions, mode, target> acc(*this, commandGroupHandler,
                                              accessRange, accessOffset);
    commandGroupHandler.alloc_mem_(acc);
    return acc;
  }

  template <access::mode mode>
  accessor<T, dimensions, mode, access::target::host_buffer>
  get_access(range<dimensions> accessRange, id<dimensions> accessOffset = {}) {
    return accessor<T, dimensions, mode, access::target::host_buffer>(
        *this, accessRange, accessOffset);
  }

  template <typename Destination = std::nullptr_t>
  void set_final_data(Destination finalData = nullptr) {
    data->set_final_data(finalData);
  }

  void set_write_back(bool flag = true);

  /* TODO: sub-buffer is not supported yet */
  bool is_sub_buffer() const {
    return false;
  }

  template <typename ReinterpretT, int ReinterpretDim>
  buffer<ReinterpretT, ReinterpretDim, AllocatorT>
  reinterpret(range<ReinterpretDim> reinterpretRange) const {
    buffer<ReinterpretT, ReinterpretDim, AllocatorT> r(*this);
    if (ReinterpretDim > get_count())
      throw sycl::invalid_object_error("invalid dimensions");
    for (int i(0); i < ReinterpretDim; i++) {
      if (r.bufferRage[i] >= reinterpretRange[i])
        r.bufferRage[i] = reinterpretRange[i];
      else
        throw sycl::invalid_object_error("invalid range");
    }
    return r;
  }

  std::shared_ptr<container_type> get_data() {
    return data;
  }

  template <access::mode mode>
  void get_futures(handler& commandGroupHandler) {
    commandGroupHandler.add_futures(fs.get_futures<mode>());
  }

  template <access::mode mode>
  void set_future(detail::shared_future_class<size_t> ft) {
    fs.refresh(ft, mode);
  }

  neosycl::sycl::detail::Futures fs;

private:
  range<dimensions> bufferRange;
  std::shared_ptr<container_type> data;
};

template <typename Ty, int D, typename A>
bool operator==(const buffer<Ty, D, A>& lhs, const buffer<Ty, D, A>& rhs) {
  return lhs.data == rhs.data;
}
template <typename Ty, int D, typename A>
bool operator!=(const buffer<Ty, D, A>& lhs, const buffer<Ty, D, A>& rhs) {
  return !(lhs == rhs);
}

} // namespace neosycl::sycl
