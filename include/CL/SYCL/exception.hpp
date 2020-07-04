//
// Created by WhiteBlue on 2020/5/25.
//

#ifndef CUSTOM_SYCL_INCLUDE_SYCL_EXCEPTION_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_EXCEPTION_HPP_

#include <exception>
#include <iterator>
#include "CL/SYCL/types.hpp"

namespace cl::sycl {

class SYCLException : public std::exception {
 public:
  const char *what() const noexcept override {
    return exception::what();
  }
};

struct exception_list : std::vector<SYCLException> {
  using std::vector<SYCLException>::vector;
};

using async_handler = function_class<void, exception_list>;

class UnimplementedException : public SYCLException {
 public:
  const char *what() const noexcept override {
    return SYCLException::what();
  }
};

class OutOfRangeException : public SYCLException {
 public:
  const char *what() const noexcept override {
    return SYCLException::what();
  }
};

class UnSupportDimensionException : public SYCLException {
 public:
  const char *what() const noexcept override {
    return SYCLException::what();
  }
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_EXCEPTION_HPP_
