#include "single_task.h"
#include <regex>

namespace sycl {

KernelInfo parse_single_task_func(CompilerInstance &ci,
                                  const FunctionDecl *callee,
                                  ProgramContext &context) {
  if (callee->getNumParams() != 1) {
    throw KernelValidateException("Single-task kernel must have 1 param");
  }
  const TemplateArgumentList *template_args =
      callee->getTemplateSpecializationArgs();
  if (template_args == nullptr || template_args->size() != 2) {
    throw KernelValidateException(
        "Single-task kernel must have 2 template args");
  }

  const TemplateArgument &classname_arg   = template_args->get(0);
  const TemplateArgument &lambda_func_arg = template_args->get(1);

  if (classname_arg.getKind() != TemplateArgument::ArgKind::Type) {
    throw KernelValidateException("Template 'ArgKind' must be 'Type'");
  }

  if (lambda_func_arg.getKind() != TemplateArgument::ArgKind::Type) {
    throw KernelValidateException("Template 'ArgKind' must be 'Type'");
  }

  QualType classname_type   = classname_arg.getAsType();
  QualType lambda_func_type = lambda_func_arg.getAsType();

#if 0
  // manginling rule could be compiler-depedent.
  // so it's better to avoid using it as the kenrel name.
  std::string mangledName;
  clang::MangleContext *mangleContext =
      ci.getASTContext().createMangleContext();
  llvm::raw_string_ostream ostream(mangledName);
  mangleContext->mangleCXXRTTI(classname_type, ostream);
  ostream.flush();
  std::string kernelName = mangledName.substr(4, mangledName.size());
#else
  std::string kernelName= classname_type.getAsString();
  std::regex re("([^\\s\\:]+)$");
  std::smatch result;
  if(std::regex_search(kernelName, result, re)){
    kernelName = result[1].str();
  }
#endif

  // Get classname here, will be used as kernel func name
  std::cout << classname_type->getTypeClassName() << std::endl;
  std::string classname =
      classname_type->getAsRecordDecl()->getDeclName().getAsString();

  CXXRecordDecl *lambda_func_decl = lambda_func_type->getAsCXXRecordDecl();

  std::vector<KernelArgument> kernel_arguments =
      analyze_arguments_dependency(ci, lambda_func_decl, context);

  CXXMethodDecl *lambda_decl = lambda_func_decl->getLambdaCallOperator();
  std::string func_body      = decl2str(ci, lambda_decl->getBody());

#ifdef DEBUG
  std::cout << "Single kernel name: " << kernelName << std::endl;
  std::cout << "========= Single Kernel body start =========" << std::endl;
  std::cout << func_body << std::endl;
  std::cout << "========= Single Kernel body end =========" << std::endl;
#endif

  KernelInfo info(kernel_arguments, kernelName, func_body);

  return info;
};

} // namespace sycl
