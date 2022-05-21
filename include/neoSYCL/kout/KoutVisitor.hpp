#pragma once
#include "Kout.hpp"
#include "KoutPrinterHelper.hpp"
#include "VarDeclFinder.hpp"

class KoutVisitor : public RecursiveASTVisitor<KoutVisitor> {
public:
  KoutVisitor(Rewriter& R, ASTContext& ast)
      : TheRewriter(R), kernCode(kcode_), ast_(ast) {}

  bool shouldVisitTemplateInstantiations() { return true; }

  void printAccessor(llvm::raw_string_ostream& st, Decl* d);
  void printVar(llvm::raw_string_ostream& st, Decl* d);
  void printLoop(llvm::raw_string_ostream& st, CXXMethodDecl* func, Decl* d,
                 int dim);

  void checkSingleTaskFunc(CXXMemberCallExpr* ce, CXXMethodDecl* callee,
                           std::string& text);
  void checkParallelForFunc(CXXMemberCallExpr* ce, CXXMethodDecl* callee,
                            std::string& text);

  bool VisitCXXMemberCallExpr(CXXMemberCallExpr* ce);

  string& getDeviceCode() { return kcode_; }

private:
  Rewriter& TheRewriter;
  string kcode_;
  raw_string_ostream kernCode;
  ASTContext& ast_;
};
