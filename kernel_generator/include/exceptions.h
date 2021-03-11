#ifndef CUSTOM_SYCL_INCLUDE_EXCEPTIONS_H_
#define CUSTOM_SYCL_INCLUDE_EXCEPTIONS_H_

namespace sycl {

class BaseException : public std::exception {
 private:
  std::string message;

 public:
  BaseException(const std::string &message) : message(message) {}

  const char *what() const noexcept override {
    return this->message.c_str();
  }
};

class ClangCastException : public BaseException {
 public:
  ClangCastException(const std::string &message) : BaseException(message) {}
};

class KernelValidateException : public BaseException {
 public:
  KernelValidateException(const std::string &message) : BaseException(message) {}
};

}

#endif //CUSTOM_SYCL_INCLUDE_EXCEPTIONS_H_
