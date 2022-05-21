#pragma once
#include "Kout.hpp"
#include "KoutPrinterHelper.hpp"
#include "VarDeclFinder.hpp"

class KoutVisitor : public RecursiveASTVisitor<KoutVisitor> {
public:
  struct KoutData {
    string handler;
    string kernel;
    string range;
    string offset;
    string func;
    string var;
    size_t dim;
    PrintingPolicy policy;
    KoutPrinterHelper helper;

    KoutData(PrintingPolicy p, ASTContext& ast)
        : dim(0), policy(p), helper(ast) {}
  };

  KoutVisitor(Rewriter& R, ASTContext& ast)
      : TheRewriter(R), kernCode(kcode_), ast_(ast) {}

  bool shouldVisitTemplateInstantiations() { return true; }

  void checkCXXMCallExpr(bool is_single, CXXMemberCallExpr* ce,
                         CXXMethodDecl* callee, std::string& text);

  bool VisitCXXMemberCallExpr(CXXMemberCallExpr* ce);

  string& getDeviceCode() { return kcode_; }

private:
  Rewriter& TheRewriter;
  string kcode_;
  raw_string_ostream kernCode;
  ASTContext& ast_;
};
