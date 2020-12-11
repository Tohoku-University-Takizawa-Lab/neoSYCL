#ifndef NEOSYCL_INCLUDE_NEOSYCL_SYCL_KERNEL_HPP_
#define NEOSYCL_INCLUDE_NEOSYCL_SYCL_KERNEL_HPP_

#include "context.hpp"
#include "program.hpp"

namespace neosycl::sycl {

class kernel {

 public:

  kernel();

  bool is_host() const;

  context get_context() const;

  program get_program() const;

};

}

#endif //NEOSYCL_INCLUDE_NEOSYCL_SYCL_KERNEL_HPP_
