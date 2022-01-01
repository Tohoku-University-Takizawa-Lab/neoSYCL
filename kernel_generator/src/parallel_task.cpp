#include "parallel_task.h"
#include <regex>

namespace sycl {

KernelInfo parse_parallel_task_func(CompilerInstance &ci,
                                    const FunctionDecl *callee,
                                    ProgramContext &context) {
  if (callee->getNumParams() != 2) {
    throw KernelValidateException("Parallel kernel must have 2 param");
  }
  const TemplateArgumentList *template_args =
      callee->getTemplateSpecializationArgs();
  if (template_args == nullptr || template_args->size() != 3) {
    throw KernelValidateException("Parallel kernel must have 3 template args");
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
  std::string mangledName;
  clang::MangleContext *mangleContext =
      ci.getASTContext().createMangleContext();
  llvm::raw_string_ostream ostream(mangledName);
  mangleContext->mangleCXXRTTI(classname_type, ostream);
  ostream.flush();
  std::string kernelName = mangledName.substr(4, mangledName.size());
#else
  std::string kernelName = classname_type.getAsString();
  std::regex re("([^\\s\\:]+)$");
  std::smatch result;
  if(std::regex_search(kernelName, result, re)){
    kernelName = result[1].str();
  }
#endif

  // Get classname here, will be used as kernel func name
  std::string classname =
      classname_type->getAsRecordDecl()->getDeclName().getAsString();

  CXXRecordDecl *lambda_func_decl = lambda_func_type->getAsCXXRecordDecl();

  std::vector<KernelArgument> kernel_arguments =
      analyze_arguments_dependency(ci, lambda_func_decl, context);

  CXXMethodDecl *lambda_decl = lambda_func_decl->getLambdaCallOperator();
  std::string func_body      = decl2str(ci, lambda_decl->getBody());

  if (lambda_decl->getNumParams() != 1) {
    throw KernelValidateException("Parallel Kernel should have Index");
  }

  std::string index_name =
      lambda_decl->getParamDecl(0)->getIdentifier()->getName().str();

#ifdef DEBUG
  std::cout << "Parallel kernel name: " << kernelName
            << " , index_name: " << index_name << std::endl;
  std::cout << "========= Parallel kernel body start =========" << std::endl;
  std::cout << func_body << std::endl;
  std::cout << "========= Parallel kernel body end =========" << std::endl;
#endif

  KernelInfo info(kernel_arguments, kernelName, func_body, index_name);

  return info;
}

} // namespace sycl