#ifndef CUSTOM_SYCL_INCLUDE_SYCL_ACCESSOR_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_ACCESSOR_HPP_

#include "CL/SYCL/types.hpp"
#include "CL/SYCL/handler.hpp"
#include "CL/SYCL/id.hpp"
#include "CL/SYCL/detail/shared_ptr_implementation.hpp"
#include "CL/SYCL/buffer/data_container.hpp"
#include "CL/SYCL/buffer/data_container_1d.hpp"
#include "CL/SYCL/buffer/data_container_2d.hpp"

namespace cl::sycl {

template<typename T, std::size_t Dimensions>
class buffer;

template<typename T,
    int Dimensions,
    access::mode mode,
    access::target accessTarget>
class accessor {
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;

 public:
  std::shared_ptr<detail::DataContainerND<T, Dimensions>> data;


  accessor(buffer<T, Dimensions> &bufferRef, handler &commandGroupHandler)
      : data(bufferRef.implementation) {
    commandGroupHandler.template add_arg<mode>(bufferRef.implementation);
  }

  explicit accessor(buffer<T, Dimensions> &bufferRef) : data(bufferRef.implementation) {
    DEBUG_INFO("copy accessor");
  }

  explicit accessor(const buffer<T, Dimensions> &bufferRef) : data(bufferRef.implementation) {
    DEBUG_INFO("copy accessor");
  }

  size_t get_size() {
    return data->get_size();
  }

  T &operator[](size_t index) {
    return data->get(index);
  }

  T &operator[](size_t index) const {
    return data->get(index);
  }

  T &operator[](const id<Dimensions> &index) {
    return data->get(index);
  }

  T &operator[](const id<Dimensions> &index) const {
    return data->get(index);
  }

  T *begin() const {
    return data->begin();
  }

  T *end() const {
    return data->end();
  }

};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_ACCESSOR_HPP_
