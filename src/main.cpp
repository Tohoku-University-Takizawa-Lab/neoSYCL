/********************************************************************
Copyright (c) 2021-2022 Hiroyuki Takizawa

This software is released under the MIT License, see LICENSE.txt.
**********************************************************************/
#include "Kout.hpp"
#include "VarDeclFinder.hpp"
#include "KoutVisitor.hpp"

static string inputfile;

static cl::OptionCategory KoutOptionCategory("More Options");

static cl::opt<string> hfile("host", cl::desc("Output host code"),
                             cl::value_desc("filename"), cl::init("-"),
                             cl::cat(KoutOptionCategory));
static cl::opt<string> dfile("device", cl::desc("Output device code"),
                             cl::value_desc("filename"), cl::init("-"),
                             cl::cat(KoutOptionCategory));

class KoutOptionsParser : public CommonOptionsParser {
public:
  KoutOptionsParser(int argc, const char** argv, cl::OptionCategory c)
      : CommonOptionsParser(argc, argv, c) {}
};

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

    ostream* os = nullptr;
    {
      ofstream fs;
      if (hfile == "-")
        os = &cout;
      else {
        fs.open(hfile, ios::out | ios::trunc);
        os = &fs;
      }
      if (buf)
        *os << std::string(buf->begin(), buf->end());
      else if (inputfile.size() > 0) {
        string tmpbuf;
        ifstream ifs(inputfile, ios::in);
        if (ifs) {
          os = &fs;
          while (getline(ifs, tmpbuf))
            *os << tmpbuf << endl;
        }
      }
    }

    string& dev = vis_.getDeviceCode();
    if (dev.size() > 0) {
      ofstream fs;
      if (dfile == "-") {
        os = &cout;
      }
      else {
        fs.open(dfile, ios::out | ios::trunc);
        os = &fs;
      }
      *os << "#define ___NEOSYCL_KERNEL_RUNTIME_ONLY___\n";
      *os << "#include \"CL/sycl.hpp\"\n\n" << dev << "\n";
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

int main(int argc, const char** argv) {
  KoutOptionsParser op(argc, argv, KoutOptionCategory);
  ClangTool tool(op.getCompilations(), op.getSourcePathList());

  if (op.getSourcePathList().size() > 0)
    inputfile = op.getSourcePathList()[0];
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
