#ifndef CUSTOM_SYCL_INCLUDE_KERNEL_TRANSLATOR_H_
#define CUSTOM_SYCL_INCLUDE_KERNEL_TRANSLATOR_H_

#include "kernel.h"
#include "fmt/format.h"

namespace sycl {

class KernelTranslator {

 public:
  virtual std::string body_to_decl_str(const ProgramContext &context, const KernelInfo &info) = 0;

  virtual std::string before_kernel(const ProgramContext &context) = 0;

  virtual std::string after_kernel(const ProgramContext &context) = 0;

};

};

#endif //CUSTOM_SYCL_INCLUDE_KERNEL_TRANSLATOR_H_
