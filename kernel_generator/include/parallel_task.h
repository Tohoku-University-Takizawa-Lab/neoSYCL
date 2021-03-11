#ifndef CUSTOM_SYCL_INCLUDE_PARALLEL_TASK_H_
#define CUSTOM_SYCL_INCLUDE_PARALLEL_TASK_H_

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

#include "exceptions.h"
#include "kernel.h"

namespace sycl {

KernelInfo parse_parallel_task_func(CompilerInstance &ci, const FunctionDecl *callee, ProgramContext &context);

}

#endif //CUSTOM_SYCL_INCLUDE_PARALLEL_TASK_H_
