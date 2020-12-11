#ifndef CUSTOM_SYCL_INCLUDE_SYCL_EXCEPTION_HPP_
#define CUSTOM_SYCL_INCLUDE_SYCL_EXCEPTION_HPP_

#include <exception>
#include <iterator>
#include "types.hpp"

namespace neosycl::sycl {

class exception : public std::exception {
 public:
  const char *what() const noexcept override {
    return exception::what();
  }
};

struct exception_list : std::vector<exception> {
  using std::vector<exception>::vector;
};

using async_handler = function_class<void, exception_list>;

class UnimplementedException : public exception {
 public:
  const char *what() const noexcept override {
    return exception::what();
  }
};

class OutOfRangeException : public exception {
 public:
  const char *what() const noexcept override {
    return exception::what();
  }
};

class UnSupportDimensionException : public exception {
 public:
  const char *what() const noexcept override {
    return exception::what();
  }
};

}

#endif //CUSTOM_SYCL_INCLUDE_SYCL_EXCEPTION_HPP_
