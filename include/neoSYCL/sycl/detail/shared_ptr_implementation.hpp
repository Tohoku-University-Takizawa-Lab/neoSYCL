#ifndef SYCL_INCLUDE_CL_SYCL_DETAIL_SHARED_PTR_IMPLEMENTATION_HPP_
#define SYCL_INCLUDE_CL_SYCL_DETAIL_SHARED_PTR_IMPLEMENTATION_HPP_

namespace neosycl::sycl::detail {

template<typename Implementation>
class SharedPtrImplementation {
 public:
  std::shared_ptr<Implementation> implementation;

  SharedPtrImplementation(std::shared_ptr<Implementation> i) : implementation{i} {}

  SharedPtrImplementation(Implementation *i) : implementation(i) {}

  SharedPtrImplementation() = default;
};
}

#endif //SYCL_INCLUDE_CL_SYCL_DETAIL_SHARED_PTR_IMPLEMENTATION_HPP_
