#pragma once

class KoutPrinterHelper : public PrinterHelper {
  ASTContext& ast_;

public:
  KoutPrinterHelper(ASTContext& a) : ast_(a) {}

  bool Visit(CXXOperatorCallExpr*, llvm::raw_ostream&);
  bool Visit(DeclRefExpr*, llvm::raw_ostream&);

  bool handledStmt(Stmt* s, llvm::raw_ostream& os) {
    // cerr << s->getStmtClassName() << endl;
    auto op = dyn_cast<CXXOperatorCallExpr>(s);
    if (op)
      return Visit(op, os);

    auto var = dyn_cast<DeclRefExpr>(s);
    if (var)
      return Visit(var, os);

    return false;
  }
};
