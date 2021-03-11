#include "kernel.h"

namespace sycl {

std::vector<KernelArgument> analyze_arguments_dependency(
    CompilerInstance &ci, const CXXRecordDecl *lambda_func_decl, ProgramContext &context) {
  std::vector<KernelArgument> args;

  // we decide the argument order by parent context
  for (Decl *d:lambda_func_decl->getParent()->decls()) {
    if (isa<VarDecl>(d)) {
      VarDecl *var = cast<VarDecl>(d);
      CXXRecordDecl *raw_decl = var->getType()->getAsCXXRecordDecl();
      if (!raw_decl) {
        continue;
      }
      std::string name = var->getIdentifier()->getName().str();

      ClassTemplateSpecializationDecl *template_decl = clang_cast<ClassTemplateSpecializationDecl>(raw_decl);
      auto template_args = template_decl->getTemplateArgs().asArray();

      if (template_args.size() != 4) {
        throw KernelValidateException("Accessor should have 4 template args");
      }

      TemplateArgument accessor_type_tmp = template_args[0];
      TemplateArgument dimensions_tmp = template_args[1];
      TemplateArgument mode_tmp = template_args[2];
      TemplateArgument target_tmp = template_args[3];

      QualType accessor_type = accessor_type_tmp.getAsType();
      int field_dimensions = dimensions_tmp.getAsIntegral().getExtValue();
      std::string field_type = accessor_type.getAsString();

      if (!accessor_type->isBuiltinType()) {
        // not builtin type
        CXXRecordDecl *type_decl = accessor_type->getAsCXXRecordDecl();
        if (type_decl) {
          if (type_decl->getIdentifier()) {
            std::string def_name = type_decl->getIdentifier()->getName().str();
            std::string def_body = decl2str(ci, type_decl);

            if (context.structs.count(def_name) == 0) {
              context.structs.insert(std::pair<std::string, std::string>(def_name, def_body));
            }

#ifdef DEBUG
            std::cout << "Definition name: " << def_name << std::endl;
            std::cout << "========= Definition body start =========" << std::endl;
            std::cout << def_body << std::endl;
            std::cout << "=========  Definition body end  =========" << std::endl;
#endif
          }
        }
      }

      args.push_back(KernelArgument{name, field_type, field_dimensions});

      PRINT_INFO("Found a lambda field decl, Type: {}, Name: {}, Dimensions: {}", field_type, name, field_dimensions);
    }
  }

  return args;
};

}


