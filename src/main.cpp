/********************************************************************
Copyright (c) 2021-2022 Hiroyuki Takizawa

This software is released under the MIT License, see LICENSE.txt.
**********************************************************************/
#include "Kout.hpp"
#include "VarDeclFinder.hpp"
#include "KoutVisitor.hpp"

class KoutASTConsumer : public ASTConsumer {
public:
  KoutASTConsumer(CompilerInstance& ci)
      : rew_(), vis_(rew_, ci.getASTContext()) {
    rew_.setSourceMgr(ci.getSourceManager(), ci.getLangOpts());
  }
  virtual void HandleTranslationUnit(clang::ASTContext& ast) {
    vis_.TraverseDecl(ast.getTranslationUnitDecl());

    const RewriteBuffer* buf =
        rew_.getRewriteBufferFor(rew_.getSourceMgr().getMainFileID());
    if (buf)
      llvm::outs() << std::string(buf->begin(), buf->end());

    string& dev = vis_.getDeviceCode();
    if (dev.size() > 0) {
      llvm::errs() << "#define ___NEOSYCL_KERNEL_RUNTIME_ONLY___\n";
      llvm::errs() << "#include \"CL/sycl.hpp\"\n\n" << dev << "\n";
    }
  }

private:
  Rewriter rew_;
  KoutVisitor vis_;
};

class KoutFrontendAction : public SyntaxOnlyAction /*ASTFrontendAction*/
{
  virtual unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& ci,
                                                    StringRef file) {
    return make_unique<KoutASTConsumer>(ci);
  }
};

static cl::OptionCategory KoutOptionCategory("More Options");

class KoutOptionsParser : public CommonOptionsParser {
public:
  KoutOptionsParser(int argc, const char** argv, cl::OptionCategory c)
      : CommonOptionsParser(argc, argv, c) {}
};

int main(int argc, const char** argv) {
  KoutOptionsParser op(argc, argv, KoutOptionCategory);
  ClangTool tool(op.getCompilations(), op.getSourcePathList());

  //  tool.clearArgumentsAdjusters();
  tool.appendArgumentsAdjuster(getClangSyntaxOnlyAdjuster());
  tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(
      "-Wno-unused-value", ArgumentInsertPosition::BEGIN));
  tool.appendArgumentsAdjuster(
      getInsertArgumentAdjuster("-std=c++17", ArgumentInsertPosition::BEGIN));
#if 0
  // avoid including g++ header files
#ifdef XEV_INCLUDE_PATH
  tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(
      XEV_INCLUDE_PATH, ArgumentInsertPosition::BEGIN));
#endif
#endif

  std::unique_ptr<FrontendActionFactory> FrontendFactory;
  FrontendFactory = newFrontendActionFactory<KoutFrontendAction>();
  return tool.run(FrontendFactory.get());
}
