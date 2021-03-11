#ifndef CUSTOM_SYCL_INCLUDE_SINGLE_TASK_H_
#define CUSTOM_SYCL_INCLUDE_SINGLE_TASK_H_

#include "clang/AST/AST.h"
#include "clang/AST/Mangle.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTImporter.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring/Extract/Extract.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

#include "exceptions.h"
#include "helpers.h"
#include "kernel.h"

using namespace clang;

namespace sycl {

KernelInfo parse_single_task_func(CompilerInstance &ci, const FunctionDecl *callee, ProgramContext &context);

}

#endif //CUSTOM_SYCL_INCLUDE_SINGLE_TASK_H_
