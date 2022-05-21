#include "KoutVisitor.hpp"

void printAccessorType(raw_string_ostream& st, Decl* d,
                       const LangOptions& opt) {
  PrintingPolicy pol(opt);
  auto vd = dyn_cast_or_null<VarDecl>(d);
  if (vd == nullptr)
    return;
  CXXRecordDecl* raw_decl = vd->getType()->getAsCXXRecordDecl();
  if (raw_decl) {
    auto* tmpl_decl = dyn_cast<ClassTemplateSpecializationDecl>(raw_decl);
    auto tmpl_args  = tmpl_decl->getTemplateArgs().asArray();
    tmpl_args[0].getAsType().print(st, pol);
  }
}

static void writeDevCode(raw_string_ostream& os, string cname, string vars,
                         string func, int dim = 0) {
  if (dim != 0) {
    os << "size_t  __" << cname << "_range__[6]={1,1,1,0,0,0};\n";
  }

  os << "class " << cname << " {\n";
  os << "public:\n";
  os << func << "\n";
  os << vars << "\n";
#if 0
  if (dim != 0) {
    os << "size_t range_(size_t i)  { return __" << cname << "_range__[i];}\n";
    os << "size_t offset_(size_t i) { return __" << cname
       << "_range__[i+3];}\n";
  }
#endif

  os << "} __" << cname << "_obj__;\n\n";

  os << "extern \"C\" {\n";
  os << "int " << cname << "() {\n";
  os << "__" << cname << "_obj__.run(";
#if 0
  if (dim != 0)
    os << "__" << cname << "_range__";
  else
    os << "nullptr";
#endif
  os << ");\n";
  os << "return 0;\n}\n";
  os << "}\n\n";
}

static void writeHostCode(raw_string_ostream& os, string cgh, string cname,
                          string vars, string func, string range_s,
                          string offset_s, VarDeclFinder& finder,
                          Rewriter& rew) {
  os << cgh << ".run<class " << cname << ">(";
  if (range_s.empty() == false) {
    os << range_s << ",";
    if (offset_s.empty() == false)
      os << offset_s << ",";
  }
  os << "[&](){\n";
  os << "class " << cname << " {\n";
  os << "public:\n";
  // os << func << "\n\n";
  os << vars << "\n";
  os << "} __" << cname << "_obj__";

  os << " {\n";
  bool first = true;
  for (int i = 0; i < finder.vlist.size(); i++) {
    auto* nd = dyn_cast<NamedDecl>(finder.vlist[i]);
    if (nd && nd->getIdentifier()) {
      if (first == false)
        os << ",";
      first = false;
      os << nd->getIdentifier()->getName();
    }
  }
  for (int i = 0; i < finder.alist.size(); i++) {
    auto* vd = dyn_cast<ValueDecl>(finder.alist[i]);
    if (vd && vd->getIdentifier()) {
      if (first == false)
        os << ",";
      first = false;
      os << cgh << ".map_(";
      os << vd->getIdentifier()->getName();
      os << ")";
    }
  }
  os << "};\n" << cgh << ".copy_capture(&__" << cname << "_obj__);\n});\n";
}

void KoutVisitor::printAccessor(raw_string_ostream& st, Decl* d) {
  auto vd = dyn_cast<VarDecl>(d);
  if (vd == nullptr || vd->getIdentifier() == nullptr)
    return;
  st << "cl::sycl::rt::acc_<";
  printAccessorType(st, vd, TheRewriter.getLangOpts());
  st << "> " << vd->getIdentifier()->getName() << ";\n";
}

void KoutVisitor::printVar(raw_string_ostream& st, Decl* d) {
  auto vd = dyn_cast<ValueDecl>(d);
  if (vd == nullptr || vd->getIdentifier() == nullptr)
    return;
  QualType t = vd->getType();
  t.removeLocalCVRQualifiers(Qualifiers::CVRMask);
  st << t.getAsString();
  st << " " << vd->getNameAsString() << ";\n";
}

void KoutVisitor::printLoop(raw_string_ostream& st, CXXMethodDecl* func,
                            Decl* d, int dim) {
  PrintingPolicy policy(TheRewriter.getLangOpts());
  KoutPrinterHelper helper(ast_);

  auto vd = dyn_cast<VarDecl>(d);
  if (vd == nullptr)
    return;
  auto vname = vd->getIdentifier()->getName();
  for (int i(dim - 1); i > -1; i--) {
    st << "size_t i" << i << "_;\n";
    st << "size_t r" << i << "_ = r_[" << i << "]  ;\n";
    st << "size_t o" << i << "_ = r_[" << i + 3 << "]  ;\n";
  }
  st << "#pragma omp parallel for\n";
  st << "#pragma _NEC ivdep\n";
  for (int i(dim - 1); i > -1; i--) {
    st << "for(i" << i << "_";
    st << "=o" << i << "_;";
    st << "i" << i << "_";
    st << " < r" << i << "_;";
    st << "i" << i << "_++)\n";
  }
  if (dim == 1)
    st << "{ cl::sycl::id<1> " << vname << "(i0_);\n";
  else if (dim == 2)
    st << "{ cl::sycl::id<2> " << vname << "(i0_,i1_);\n";
  else if (dim == 3)
    st << "{ cl::sycl::id<3> " << vname << "(i0_,i1_,i2_);\n";

  func->getBody()->printPretty(st, &helper, policy);

  st << "}";
}

void KoutVisitor::checkSingleTaskFunc(CXXMemberCallExpr* ce,
                                      CXXMethodDecl* callee,
                                      std::string& text) {
  /* sycl::handler::single_task should have 1 arg and 2 template args. */
  if (callee->getNumParams() != 1)
    return;
  auto* template_args = callee->getTemplateSpecializationArgs();
  if (template_args == nullptr || template_args->size() != 2)
    return;

  /* 2 template args of typename. */
  const TemplateArgument& a0 = template_args->get(0);
  const TemplateArgument& a1 = template_args->get(1);
  if (a0.getKind() != TemplateArgument::ArgKind::Type)
    return;
  if (a1.getKind() != TemplateArgument::ArgKind::Type)
    return;

  QualType kernel_name        = a0.getAsType();
  QualType functor_type       = a1.getAsType();
  CXXRecordDecl* functor_decl = functor_type->getAsCXXRecordDecl();
  CXXMethodDecl* functor_op   = functor_decl->getLambdaCallOperator();

  string caller_func = kernel_name.getAsString();
  if (kernel_name.getBaseTypeIdentifier()) // this should be true
    caller_func = kernel_name.getBaseTypeIdentifier()->getNameStart();

  string func, vars;
  VarDeclFinder finder;
  PrintingPolicy policy(TheRewriter.getLangOpts());
  KoutPrinterHelper helper(ast_);

  if (functor_op->getBody()) {
    raw_string_ostream st(func);
    st << "void run()\n";
    functor_op->getBody()->printPretty(st, &helper, policy);

    finder.TraverseStmt(functor_op->getBody());
    if (finder.vlist.empty() == false || finder.alist.empty() == false) {
      raw_string_ostream st(vars);
      for (const auto& it : finder.vlist) {
        printVar(st, it);
      }
      for (const auto& it : finder.alist) {
        printAccessor(st, it);
      }
    }
  }
  else
    cerr << "No function body\n";

  string cgh;
  raw_string_ostream os(text);
  raw_string_ostream cghs(cgh);
  ce->getImplicitObjectArgument()->printPretty(cghs, NULL, policy);

  os << "\n// --- BEGIN---\n";

  writeHostCode(os, cgh, caller_func, vars, func, "", "", finder, TheRewriter);

  os << "\n// --- END ---\n";

  writeDevCode(kernCode, caller_func, vars, func);
}

#if 0
/* The following functions are assumed */
template <typename KernelName, typename KernelType>
void single_task(KernelType kernelFunc);

template <typename KernelName, typename KernelType, int dimensions>
void parallel_for(range<dimensions> numWorkItems, KernelType kernelFunc);

template <typename KernelName, typename KernelType, int dimensions>
void parallel_for(range<dimensions> numWorkItems,
id<dimensions> workItemOffset, KernelType kernelFunc);
/* nd_range is not supprted by the runtime */
template <typename KernelName, typename KernelType, int dimensions>
void parallel_for(nd_range<dimensions> executionRange, KernelType kernelFunc);
#endif

void KoutVisitor::checkParallelForFunc(CXXMemberCallExpr* ce,
                                       CXXMethodDecl* callee,
                                       std::string& text) {

  /* sycl::handler::parallel_for should have 2 args and 3 template args. */
  if (callee->getNumParams() < 2)
    return;
  auto* template_args = callee->getTemplateSpecializationArgs();
  if (template_args == nullptr || template_args->size() != 3)
    return;

  /* 3 template args of typename. */
  const TemplateArgument& a0 = template_args->get(0);
  const TemplateArgument& a1 = template_args->get(1);
  const TemplateArgument& a2 = template_args->get(2);
  if (a0.getKind() != TemplateArgument::ArgKind::Type)
    return;
  if (a1.getKind() != TemplateArgument::ArgKind::Type)
    return;
  if (a2.getKind() != TemplateArgument::ArgKind::Integral)
    return;

  QualType kernel_name        = a0.getAsType();
  QualType functor_type       = a1.getAsType();
  llvm::APSInt dim_int        = a2.getAsIntegral();
  CXXRecordDecl* functor_decl = functor_type->getAsCXXRecordDecl();
  CXXMethodDecl* functor_op   = functor_decl->getLambdaCallOperator();

  size_t dim         = dim_int.getExtValue();
  string caller_func = kernel_name.getAsString();
  if (kernel_name.getBaseTypeIdentifier()) // this should be true
    caller_func = kernel_name.getBaseTypeIdentifier()->getNameStart();

  string func, vars;
  VarDeclFinder finder;
  PrintingPolicy policy(TheRewriter.getLangOpts());

  if (functor_op->getBody()) {
    finder.TraverseStmt(functor_op->getBody());
    if (finder.vlist.empty() == false || finder.alist.empty() == false) {
      raw_string_ostream st(vars);
      for (const auto& it : finder.vlist) {
        printVar(st, it);
      }
      for (const auto& it : finder.alist) {
        printAccessor(st, it);
      }
    }

    raw_string_ostream st(func);
    st << "void run(){\n";
    st << "size_t* r_ = __" << caller_func << "_range__\n;";
    /*
      for (const auto &it : finder.parm_list) {
          printVar(st, it);
      }
      */
    printLoop(st, functor_op, finder.parm_list[0], dim);
    st << "}\n";
  }
  else {
    cerr << "No function body\n";
    return;
  }

  string cgh, rng, off;
  raw_string_ostream os(text);
  raw_string_ostream cghs(cgh);
  raw_string_ostream rngs(rng);
  raw_string_ostream offs(off);
  ce->getImplicitObjectArgument()->printPretty(cghs, NULL, policy);
  ce->getArg(0)->printPretty(rngs, NULL, policy);
  if (ce->getNumArgs() == 3)
    ce->getArg(1)->printPretty(offs, NULL, policy);

  os << "\n// --- BEGIN---\n";

  writeHostCode(os, cgh, caller_func, vars, func, rng, off, finder,
                TheRewriter);

  os << "\n// --- END ---\n";

  writeDevCode(kernCode, caller_func, vars, func, dim);
  return;
}

void printObjName(CXXMemberCallExpr* ce, string& text, PrintingPolicy pol) {
  raw_string_ostream os(text);
}

bool KoutVisitor::VisitCXXMemberCallExpr(CXXMemberCallExpr* ce) {
  if (ce->getDirectCallee() == nullptr)
    return true;

  auto mdecl = dyn_cast<CXXMethodDecl>(ce->getDirectCallee());
  if (mdecl == nullptr || mdecl->isStatic())
    return true; // ignore static methods

  string fname = mdecl->getNameAsString();
  string cname = mdecl->getThisObjectType().getAsString();
  string class_def;

  if (mdecl->getParent()->bases_begin()) {
    cname = mdecl->getParent()->bases_begin()->getType().getAsString();
  }
  // cerr << cname << "::" << fname << endl;
  if (cname == SYCL_HANDLER) {
    printObjName(ce, class_def, TheRewriter.getLangOpts());
    if (fname == SYCL_SINGLE_TASK)
      checkSingleTaskFunc(ce, mdecl, class_def);
    else if (fname == SYCL_PARALLEL_FOR)
      checkParallelForFunc(ce, mdecl, class_def);
  }

  if (class_def.empty() == false) {
    //    TheRewriter.InsertTextBefore(ce->getSourceRange().getBegin(),
    //    class_def);
    TheRewriter.ReplaceText(ce->getSourceRange(), class_def);
  }

  return true;
}

#if 0
bool VisitFunctionDecl(FunctionDecl* f)
{
  // Only function definitions (with bodies), not declarations.
  if (f->hasBody())
  {
    Stmt* FuncBody = f->getBody();

    // Type name as string
    QualType QT = f->getReturnType();
    std::string TypeStr = QT.getAsString();

    // Function name
    DeclarationName DeclName = f->getNameInfo().getName();
    std::string FuncName = DeclName.getAsString();

    // Add comment before
    std::stringstream SSBefore;
    SSBefore << "// Begin function " << FuncName << " returning " << TypeStr
      << "\n";
    SourceLocation ST = f->getSourceRange().getBegin();
    TheRewriter.InsertText(ST, SSBefore.str(), true, true);

    // And after
    std::stringstream SSAfter;
    SSAfter << "\n// End function " << FuncName;
    ST = FuncBody->getEndLoc().getLocWithOffset(1);
    TheRewriter.InsertText(ST, SSAfter.str(), true, true);
  }

  return true;
}
#endif
