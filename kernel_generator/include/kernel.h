//
// Created by WhiteBlue on 2020/5/20.
//

#ifndef CUSTOM_SYCL_RUNTIME_KERNEL_HPP_
#define CUSTOM_SYCL_RUNTIME_KERNEL_HPP_

#include <string>
#include <utility>
#include <vector>
#include <iostream>

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTImporter.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring/Extract/Extract.h"
#include "clang/Tooling/Tooling.h"

#include "helpers.h"

#include "fmt/format.h"

using namespace clang;

namespace sycl {

struct KernelArgument {
  std::string name;
  std::string type;
  int dimensions;
};

struct KernelInfo {
  std::vector<KernelArgument> params;
  std::string kernel_name;
  std::string kernel_body;
  std::string index_name;
  bool parallel;

  KernelInfo(std::vector<KernelArgument> params,
             std::string kernel_name,
             std::string kernel_body)
      : params(std::move(params)),
        kernel_name(std::move(kernel_name)),
        kernel_body(std::move(kernel_body)),
        parallel(false) {}

  KernelInfo(std::vector<KernelArgument> params,
             std::string kernel_name,
             std::string kernel_body,
             std::string index_name)
      : params(std::move(params)),
        kernel_name(std::move(kernel_name)),
        kernel_body(std::move(kernel_body)),
        index_name(std::move(index_name)),
        parallel(true) {}
};

struct ProgramContext {
  std::map<std::string, KernelInfo> kernels;
  std::map<std::string, std::string> structs;
};

std::vector<KernelArgument> analyze_arguments_dependency(CompilerInstance &ci,
                                                         const CXXRecordDecl *lambda_func_decl,
                                                         ProgramContext &context);

}

#endif //CUSTOM_SYCL_RUNTIME_KERNEL_HPP_
