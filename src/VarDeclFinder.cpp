#include "VarDeclFinder.hpp"

bool VarDeclFinder::VisitDecl(Decl* d) {
  decl_list.push_back(d);
  return true;
}

bool VarDeclFinder::VisitDeclRefExpr(DeclRefExpr* e) {
  NamedDecl* d = e->getFoundDecl();
  //cerr << d->getDeclKindName() << " " << d->getNameAsString() << endl;
  if (isInList(decl_list, d) == true) {
    return true; // declared in the kernel func
  }
  if (isInList(vlist, d) == true) {
    return true;
  }
  if (isInList(alist, d) == true) {
    return true;
  }
  if (isInList(parm_list, d) == true) {
    return true;
  }

  auto* fd = dyn_cast<FunctionDecl>(d);
  if (fd) {
    if (fd->getDefinition())
      fd = fd->getDefinition();
    if (fd->hasBody())
      vlist.push_back(fd);
    else
      cerr << "[WARN] no function body found\n";
    return true;
  }

  // check if it is the first argument of parallel_for
  auto* pd = dyn_cast<ParmVarDecl>(d);
  if (pd && pd->getIdentifier()) {
    decl_list.push_back(pd);
    parm_list.push_back(pd);
    return true;
  }

  auto* vd = dyn_cast<ValueDecl>(d);
  if (vd && vd->getIdentifier()) {
    string type_name = vd->getType().getAsString();

    bool acc = isAccessor(type_name);
    if (acc == true) {
      alist.push_back(vd);
    }
    else {
      vlist.push_back(d);
    }
  }
  return true;
}

bool VarDeclFinder::VisitMemberExpr(MemberExpr* e) {
  Expr* b = e->getBase();
  if (dyn_cast<CXXThisExpr>(b) == nullptr)
    return true; // not converted

  ValueDecl* d = e->getMemberDecl();
  if (isInList(vlist, d) == true)
    return true;
#if 0 
  // not implemented yet
  auto *md = dyn_cast<CXXMethodDecl>(d);
  if (md) {
    if (md->getDefinition())
      md = dyn_cast<CXXMethodDecl>(md->getDefinition());
    if (md->hasBody())
      vlist.push_back(md);
    else
      cerr << "[WARN] no function body found\n";
    return true;
  }
#endif
  //auto* nd = dyn_cast<NamedDecl>(d);
  if (d && d->getIdentifier()) {
#if 0
    QualType t = d->getType();
    t.removeLocalCVRQualifiers(Qualifiers::CVRMask);
    cerr << t.getAsString();
    cerr << " " << d->getNameAsString() << ";\n";
#endif
    vlist.push_back(d);
  }

  return true;
}
