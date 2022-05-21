#pragma once
#include "Kout.hpp"

class VarDeclFinder : public RecursiveASTVisitor<VarDeclFinder> {
public:
  virtual bool shouldVisitTemplateInstantiations() { return true; }

  bool isInList(std::vector<Decl*>& l, Decl* d) {
    for (const auto& item : l) {
      if (item == d) {
        return true;
      }
    }
    return false;
  }

  bool isAccessor(std::string& type_name) {
    std::regex re("(" + string(SYCL_ACCESSOR) + ")<.*>");
    std::smatch result;
    if (std::regex_search(type_name, result, re)) {
      type_name = result[1].str();
      return true;
    }
    return false;
  }

  bool VisitDecl(Decl* d);
  bool VisitDeclRefExpr(DeclRefExpr* e);
  bool VisitMemberExpr(MemberExpr* e);

  std::vector<Decl*> vlist;
  std::vector<Decl*> alist;
  std::vector<Decl*> decl_list;
  std::vector<Decl*> parm_list;
};
