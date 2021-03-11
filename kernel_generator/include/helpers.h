#ifndef CUSTOM_SYCL_INCLUDE_HELPERS_H_
#define CUSTOM_SYCL_INCLUDE_HELPERS_H_

#include <exception>

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

#include "exceptions.h"

#include <iostream>
#include <fmt/core.h>

#ifdef DEBUG
#define PRINT_DEBUG(...) std::cout<< "[DEBUG] "<< fmt::format(__VA_ARGS__) <<std::endl
#else
#define DEBUG_INFO(format, ...)
#endif
#define PRINT_INFO(...) std::cout<< "[INFO] "<< fmt::format(__VA_ARGS__) <<std::endl
#define PRINT_INFO_RAW(...) std::cout<< "[INFO] "<< __VA_ARGS__ <<std::endl

using namespace clang;

namespace sycl {

static const std::string LINE_BREAK = "\n";

template<typename T>
std::string decl2str(CompilerInstance &ci, T *d) {
  SourceManager &sm = ci.getSourceManager();
  SourceLocation start(d->getBeginLoc()), end(d->getEndLoc());
  SourceLocation e(Lexer::getLocForEndOfToken(end, 0, sm, ci.getLangOpts()));
  return std::string(sm.getCharacterData(start), sm.getCharacterData(e) - sm.getCharacterData(start));
}

template<typename T, typename N>
T *clang_cast(N any) {
  if (isa<T>(any)) {
    return cast<T>(any);
  } else {
    any->dump();
    throw ClangCastException("Clang node cast failed");
  }
}

}

#endif //CUSTOM_SYCL_INCLUDE_HELPERS_H_
