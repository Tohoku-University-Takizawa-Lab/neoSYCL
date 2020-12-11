#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_INFO_PROGRAM_HPP_
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_INFO_PROGRAM_HPP_

namespace neosycl::sycl {

namespace info {

enum class program : int {
  reference_count,
  context,
  devices
};


}
}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_INFO_PROGRAM_HPP_
