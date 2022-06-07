#include "KoutVisitor.hpp"

using str  = raw_string_ostream;
using Data = KoutVisitor::KoutData;

static bool isInMainFile(Decl* d, SourceManager& smgr) {
  // if (d && smgr.isInMainFile(smgr.getExpansionLoc(d->getLocation())))
  if (d && smgr.isInMainFile(d->getBeginLoc()))
    return true;
  return false;
}

static bool isInMainFile(Decl* d, Data& data) {
  return isInMainFile(d, data.smgr);
}

static void printAccessor(str& st, Decl* d) {
  auto vd = dyn_cast_or_null<VarDecl>(d);
  if (vd == nullptr || vd->getIdentifier() == nullptr)
    return;
  CXXRecordDecl* raw_decl = vd->getType()->getAsCXXRecordDecl();
  if (raw_decl == nullptr)
    return;
  auto* tmpl_decl = dyn_cast<ClassTemplateSpecializationDecl>(raw_decl);

  auto tmpl_args = tmpl_decl->getTemplateArgs().asArray();
  auto t         = tmpl_args[0].getAsType();
  st << "cl::sycl::rt::acc_<";
  st << t.getAsString();
  st << "> " << vd->getIdentifier()->getName() << ";\n";
}

static void printVar(str& st, Decl* d, Data& data) {
  auto fd = dyn_cast_or_null<FunctionDecl>(d);
  if (fd) {
    if (isInMainFile(fd, data) && fd->isInlined() == false)
      d->print(st, data.policy);
    return;
  }
  auto vd = dyn_cast_or_null<ValueDecl>(d);
  if (vd == nullptr || vd->getIdentifier() == nullptr)
    return;
  QualType t = vd->getType();

  auto ty = dyn_cast<ConstantArrayType>(t.getTypePtr());
  if (ty == nullptr) {
    t.removeLocalCVRQualifiers(Qualifiers::CVRMask);
    st << t.getAsString();
    st << " " << vd->getNameAsString() << ";\n";
  }
  else {
    QualType et = ty->getElementType();
    et.removeLocalCVRQualifiers(Qualifiers::CVRMask);
    st << et.getAsString();
    st << " " << vd->getNameAsString();
    st << "[";
    ty->getSize().print(st, false);
    st << "];\n";
  }
}

static void printVarDecls(CXXRecordDecl* functor_decl, Data& data,
                          VarDeclFinder& finder) {
  str st(data.var);
  // print this->members and functions
  for (const auto& it : finder.vlist) {
    printVar(st, it, data);
  }

  // print captured variables
  for (auto& i : functor_decl->captures()) {
    if (i.capturesVariable() == false) {
      continue; // ignore "this" is captured.
    }
    VarDecl* decl = i.getCapturedVar();
    if (decl) {
      QualType t = decl->getType();
      auto ti    = t.getBaseTypeIdentifier();
      if (ti && ti->getNameStart() == string("accessor"))
        data.alist.push_back(&i);
      else
        data.vlist.push_back(&i);
    }
  }
  for (const auto& i : data.vlist) {
    printVar(st, i->getCapturedVar(), data);
  }
  for (const auto& i : data.alist) {
    printAccessor(st, i->getCapturedVar());
  }
}

static void printLoop(str& st, CXXMethodDecl* func, Decl* d, Data& data) {
  const char* pragma_omp_parallel_for = "#pragma omp parallel for\n";
  const char* pragma_NEC_ivdep        = "#pragma _NEC ivdep\n";

  int dim = data.dim;

  auto vd = dyn_cast_or_null<VarDecl>(d);
  if (vd == nullptr || vd->getIdentifier() == nullptr)
    return;
  auto vname = vd->getIdentifier()->getName();
  auto vtype = string(vd->getType().getBaseTypeIdentifier()->getNameStart());
  for (int i(dim - 1); i > -1; i--) {
    st << "size_t i" << i << "_;\n";
    st << "size_t r" << i << "_ = r_[" << i << "]  ;\n";
    st << "size_t o" << i << "_ = r_[" << i + 3 << "]  ;\n";
  }
  st << pragma_omp_parallel_for;
  st << pragma_NEC_ivdep;
  for (int i(dim - 1); i > -1; i--) {
    st << "for(i" << i << "_";
    st << "=o" << i << "_;";
    st << "i" << i << "_";
    st << " < r" << i << "_;";
    st << "i" << i << "_++)\n";
  }
  if (vtype == "id") {
    if (dim == 1)
      st << "{ [[maybe_unused]] cl::sycl::id<1> " << vname << "(i0_);\n";
    else if (dim == 2)
      st << "{ [[maybe_unused]] cl::sycl::id<2> " << vname << "(i0_,i1_);\n";
    else if (dim == 3)
      st << "{ [[maybe_unused]] cl::sycl::id<3> " << vname << "(i0_,i1_,i2_);\n";
  }
  else if (vtype == "item") {
    if (dim == 1)
      st << "{ [[maybe_unused]] cl::sycl::item<1> " << vname
         << "= cl::sycl::rt::id2item(r_,i0_);\n";
    else if (dim == 2)
      st << "{ [[maybe_unused]] cl::sycl::item<2> " << vname
         << "= cl::sycl::rt::id2item(r_,i0_,i1_);\n";
    else if (dim == 3)
      st << "{ [[maybe_unused]] cl::sycl::item<3> " << vname
         << "= cl::sycl::rt::id2item(r_,i0_,i1_,i2_);\n";
  }
  else {
    cerr << "unknown index class\n";
    abort();
  }
  func->getBody()->printPretty(st, &data.helper, data.policy);

  st << "}";
}

static void printRunFunc(CXXMethodDecl* functor_op, Data& data) {
  str st(data.func);
  st << "void run()\n";
  if (data.dim == 0) {
    assert(functor_op->getBody() != nullptr);
    functor_op->getBody()->printPretty(st, &data.helper, data.policy);
  }
  else {
    st << "{ size_t* r_ = __" << data.kernel << "_range__\n;";
    assert(functor_op->getNumParams() > 0);
    printLoop(st, functor_op, functor_op->getParamDecl(0), data);
    st << "}\n";
  }
}

static void writeDevCode(str& os, Data& data) {
  string& cls = data.kernel;
  //if (data.dim != 0)
    os << "size_t  __" << cls << "_range__[6]={1,1,1,0,0,0};\n";

  os << "class " << cls << " {\n";
  os << "public:\n";
  os << data.func << "\n";
  os << data.var << "\n";
  os << "} __" << cls << "_obj__;\n\n";

  os << "extern \"C\" {\n";
  os << "int " << cls << "() {\n";
  os << "__" << cls << "_obj__.run(";
  os << ");\n";
  os << "return 0;\n}\n";
  os << "}\n\n";
}

static void writeHostCode(str& os, Data& data, VarDeclFinder& finder) {
  string& cls = data.kernel;
  os << data.handler << ".run<class " << cls << ">(";
  if (data.dim > 0 && data.range.empty() == false) {
    os << data.range << ",";
    if (data.offset.empty() == false)
      os << data.offset << ",";
  }
  os << "[&](){\n";
  os << "class " << cls << " {\n";
  os << "public:\n";
  os << data.var << "\n";
  os << "} __" << cls << "_obj__";

  os << " {\n";
  bool first = true;
  for (int i = 0; i < finder.vlist.size(); i++) {
    auto* fd = dyn_cast_or_null<FunctionDecl>(finder.vlist[i]);
    if (fd) // ignore functions
      continue;
    auto* nd = dyn_cast_or_null<NamedDecl>(finder.vlist[i]);
    if (nd && nd->getIdentifier()) {
      if (first == false)
        os << ",";
      first = false;
      os << nd->getIdentifier()->getName();
    }
  }
  for (int i = 0; i < data.vlist.size(); i++) {
    if (data.vlist[i]->capturesVariable() == false) {
      continue; // ignore "this" is captured.
    }
    if (first == false)
      os << ",";
    first  = false;
    auto d = data.vlist[i]->getCapturedVar();
    if (data.vlist[i]->isExplicit() && d->hasInit()) {
      d->getInit()->printPretty(os, &data.helper, data.policy);
    }
    else
      os << d->getNameAsString();
  }
  for (int i = 0; i < data.alist.size(); i++) {
    if (first == false)
      os << ",";
    first    = false;
    auto* vd = data.alist[i]->getCapturedVar();
    os << data.handler << ".map_(";
    os << vd->getIdentifier()->getName();
    os << ")";
  }
  os << "};\n";
  os << "return std::make_tuple((void*)&__" << cls << "_obj__,";
  os << "sizeof(__" << cls << "_obj__));\n});\n";
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

void KoutVisitor::checkCXXMCallExpr(bool is_single, CXXMemberCallExpr* ce,
                                    CXXMethodDecl* callee, std::string& text) {

  auto* template_args = callee->getTemplateSpecializationArgs();
  if (template_args == nullptr)
    return;

  /* sycl::handler::single_task: 1 arg and 2 temp args. */
  if (is_single) {
    if (callee->getNumParams() != 1 || template_args->size() != 2)
      return;
  }
  else {
    /* sycl::handler::parallel_for: 2 or 3 args and 3 temp args. */
    if (callee->getNumParams() < 2 || template_args->size() != 3)
      return;
  }

  /* 2 template args of typename: KernelName and KernelType */
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

  /* the last template arg of parallel_for */
  int dim = 0;
  if (!is_single) {
    const TemplateArgument& a2 = template_args->get(2);
    if (a2.getKind() != TemplateArgument::ArgKind::Integral)
      return;
    dim = a2.getAsIntegral().getExtValue();
  }

  KoutData data(PrintingPolicy(TheRewriter.getLangOpts()), ast_);

  data.dim    = dim;
  data.kernel = kernel_name.getAsString();
  if (kernel_name.getBaseTypeIdentifier()) // this should be true
    data.kernel = kernel_name.getBaseTypeIdentifier()->getNameStart();
  if (ce->getImplicitObjectArgument()) { // this should be true
    str cghs(data.handler);
    ce->getImplicitObjectArgument()->printPretty(cghs, NULL, data.policy);
  }
  else {
    cerr << "Handler identifier not found." << endl;
    abort();
  }

  VarDeclFinder finder;
  if (functor_op->getBody()) {
    for (auto& i : functor_decl->captures()) {
      if (i.capturesVariable() == true)
        finder.decl_list.push_back(i.getCapturedVar());
    }
    finder.TraverseStmt(functor_op->getBody());
  }
  else {
    cerr << "No function body\n";
    return;
  }

  // print variable decls in the class
  printVarDecls(functor_decl, data, finder);

  // print the run function in the class
  printRunFunc(functor_op, data);

  if (is_single == false) {
    str rngs(data.range);
    str offs(data.offset);
    ce->getArg(0)->printPretty(rngs, NULL, data.policy);
    if (ce->getNumArgs() == 3)
      ce->getArg(1)->printPretty(offs, NULL, data.policy);
  }

  str os(text);
  os << "\n// --- BEGIN---\n";

  writeHostCode(os, data, finder);

  os << "\n// --- END ---\n";

  writeDevCode(kernCode, data);
  return;
}

bool KoutVisitor::VisitCXXMemberCallExpr(CXXMemberCallExpr* ce) {
  auto mdecl = dyn_cast_or_null<CXXMethodDecl>(ce->getDirectCallee());
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
    if (fname == SYCL_SINGLE_TASK)
      checkCXXMCallExpr(true, ce, mdecl, class_def);
    else if (fname == SYCL_PARALLEL_FOR)
      checkCXXMCallExpr(false, ce, mdecl, class_def);
  }

  if (class_def.empty() == false) {
    // TheRewriter.ReplaceText(ce->getSourceRange(), class_def);
    TheRewriter.InsertTextBefore(ce->getSourceRange().getBegin(), class_def);
  }

  return true;
}

bool KoutVisitor::VisitTypeAliasDecl(TypeAliasDecl* d) {
  PrintingPolicy policy(TheRewriter.getLangOpts());
  SourceManager& smgr = ast_.getSourceManager();

  // now only global declaratios are considered
  if (d->getParentFunctionOrMethod() != nullptr)
    return true;
  // TODO: non-global ones

  if (isInMainFile(d, smgr)) {
    d->print(kernCode, policy);
    kernCode << ";\n";
  }
  return true;
}
