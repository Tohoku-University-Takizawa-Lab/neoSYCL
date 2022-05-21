#include "Kout.hpp"
#include "KoutPrinterHelper.hpp"

static const char* neosycl_id = "neosycl::sycl::id";
static const char* neosycl_ac = "neosycl::sycl::accessor";

bool KoutPrinterHelper::Visit(DeclRefExpr* e, llvm::raw_ostream& os) {
  auto decl = dyn_cast<ValueDecl>(e->getFoundDecl());
  auto t    = e->getType();
  if (decl == nullptr || t.getBaseTypeIdentifier() == nullptr)
    return false;
  const char* name = t.getBaseTypeIdentifier()->getNameStart();
  /* TODO: consider namespaces */
  if (name == nullptr || string(name) != "id")
    return false;

  CXXRecordDecl* raw_decl = decl->getType()->getAsCXXRecordDecl();
  if (raw_decl) {
    auto* tmpl_decl = dyn_cast<ClassTemplateSpecializationDecl>(raw_decl);
    auto tmpl_args  = tmpl_decl->getTemplateArgs().asArray();
    auto ival       = tmpl_args[0].getAsIntegral().getExtValue();
    if (ival != 1)
      return false;
    os << "i0_";
    return true;
  }

  return false;
}

bool KoutPrinterHelper::Visit(CXXOperatorCallExpr* op, llvm::raw_ostream& os) {

  if (op == nullptr || op->getNumArgs() < 2)
    return false;

  auto arg0 = op->getArg(0); // base
  auto arg1 = op->getArg(1); // index

  // check if the function is operator[]
  auto fd = op->getDirectCallee();
  auto md = dyn_cast_or_null<CXXMethodDecl>(fd);
  if (md == nullptr || md->getNameAsString() != "operator[]")
    return false;

  auto parent = md->getParent(); // CXXRecordDecl
  if (parent->getQualifiedNameAsString() == neosycl_id) {
    /* id::operator[] */
    Expr::EvalResult result;
    if (arg1->EvaluateAsInt(result, ast_) == false)
      return false;
    os << "i" << result.Val.getInt().getExtValue() << "_";
    return true;
  }
  else if (parent->getQualifiedNameAsString() == neosycl_ac) {
    /* accessor::operator[] */
  }
  return false;
}
