#ifndef CUSTOM_SYCL_INCLUDE_VE_KERNEL_TRANSLATOR_H_
#define CUSTOM_SYCL_INCLUDE_VE_KERNEL_TRANSLATOR_H_

#include "kernel_translator.h"

namespace sycl {

class VEKernelTranslator : public KernelTranslator {
 public:
  std::string body_to_decl_str(const ProgramContext &context, const KernelInfo &info) override;

  std::string before_kernel(const ProgramContext &context) override;

  std::string after_kernel(const ProgramContext &context) override;

};

}

#endif //CUSTOM_SYCL_INCLUDE_VE_KERNEL_TRANSLATOR_H_
