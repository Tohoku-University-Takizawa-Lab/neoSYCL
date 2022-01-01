#include "ve_kernel_translator.h"

namespace sycl {

std::string VEKernelTranslator::body_to_decl_str(const ProgramContext &context,
                                                 const KernelInfo &info) {
  std::string func_params;

  // generate function params
  for (const KernelArgument &arg : info.params) {
    func_params += fmt::format("{} *{}, ", arg.type, arg.name);
  }

  if (info.parallel) {
    // generate parallel for kernel here
    std::string body = fmt::format("\nfor(int {0}=0;{0}<N;{0}+=STEP){1}\n",
                                   info.index_name, info.kernel_body);
    std::string ret =
        fmt::format("int {}({}int N, int STEP){}{}return 0;\n{}",
                    info.kernel_name, func_params, "{", body, "}");
    return ret;
  } else {
    // generate single-task kernel here
    std::string body = info.kernel_body;
    std::string ret  = fmt::format(
         "int {}({}){}{}return 0;\n{}", info.kernel_name,
         func_params.substr(0, func_params.size() - 2), "{", body, "}");
    return ret;
  }
}

std::string VEKernelTranslator::before_kernel(const ProgramContext &context) {
  std::string ret;
  ret.append("#include <math.h>").append(LINE_BREAK);
  ret.append("#include <stdio.h>").append(LINE_BREAK);
  for (auto &def : context.structs) {
    ret.append(def.second).append(";").append(LINE_BREAK);
  }
  return ret;
}

std::string VEKernelTranslator::after_kernel(const ProgramContext &context) {
  return "";
}

} // namespace sycl
