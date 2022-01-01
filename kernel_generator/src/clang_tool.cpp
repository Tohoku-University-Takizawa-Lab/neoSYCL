#include <fstream>
#include <memory>
#include <map>

#include "clang/AST/AST.h"
#include "clang/AST/Mangle.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring/Extract/Extract.h"
#include "clang/Tooling/Tooling.h"
#include "fmt/format.h"

#include "kernel.h"
#include "parallel_task.h"
#include "single_task.h"
#include "ve_kernel_translator.h"

static llvm::cl::OptionCategory MyToolCategory("Additional options");
static llvm::cl::opt<std::string>
    OutputNameOption("o", llvm::cl::desc("output filename"),
                     llvm::cl::value_desc("filename"),
                     llvm::cl::cat(MyToolCategory));

static llvm::cl::extrahelp
    CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp("SYCL Kernel generate tool");

const static std::string KERNEL_HIGHLIGHT_SINGLE_TASK_FUNC_NAME =
"single_task";
    // "HIGHLIGHT_KERNEL_SINGLE_TASK";
const static std::string KERNEL_HIGHLIGHT_PARALLEL_FUNC_NAME =
"parallel_for";
    //"HIGHLIGHT_KERNEL_PARALLEL";
const static std::string DEFAULT_OUTPUT_NAME = "kernel";

using namespace sycl;

class SYCLVisitor : public clang::RecursiveASTVisitor<SYCLVisitor> {
private:
  clang::Rewriter &rewriter;
  clang::SourceManager &manager;
  clang::CompilerInstance &instance;
  ProgramContext context;

public:
  SYCLVisitor(clang::CompilerInstance &ci, clang::SourceManager &sm,
              clang::Rewriter &re)
      : instance(ci), rewriter(re), manager(sm) {}

  virtual bool shouldVisitTemplateInstantiations() { return true; }

  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    // For debugging, dumping the AST nodes will show which nodes are already
    // being visited.
    Declaration->getDeclName().getUsingDirectiveName();

    // The return value indicates whether we want the visitation to proceed.
    // Return false to stop the traversal of the AST.
    return true;
  }

  bool VisitStmt(clang::Stmt *s) {
    try {

      if (clang::isa<clang::CallExpr>(s)) {
        clang::CallExpr *call_expr  = clang::cast<clang::CallExpr>(s);
        clang::FunctionDecl *callee = call_expr->getDirectCallee();
        if (callee && callee->getIdentifier()) {
          // Get the func which name start with SYCL_PREFIX
          if (callee->getName().compare(
                  KERNEL_HIGHLIGHT_SINGLE_TASK_FUNC_NAME) == 0) {
            KernelInfo info =
                parse_single_task_func(this->instance, callee, context);
            if (context.kernels.count(info.kernel_name) == 0) {
              context.kernels.insert(
                  std::pair<std::string, KernelInfo>(info.kernel_name, info));
            }
          } else if (callee->getName().compare(
                         KERNEL_HIGHLIGHT_PARALLEL_FUNC_NAME) == 0) {
            KernelInfo info =
                parse_parallel_task_func(this->instance, callee, context);
            if (context.kernels.count(info.kernel_name) == 0) {
              context.kernels.insert(
                  std::pair<std::string, KernelInfo>(info.kernel_name, info));
            }
          }
        }
      }
    } catch (BaseException e) {
      llvm::errs() << e.what() << "\n";
      return true;
    }
    return true;
  }

  ProgramContext get_context() { return context; }
};

class SYCLASTConsumer : public clang::ASTConsumer {
private:
  std::unique_ptr<SYCLVisitor> visitor;
  clang::Rewriter rewriter;
  clang::SourceManager &manager;
  clang::CompilerInstance &instance;
  VEKernelTranslator translator;

public:
  explicit SYCLASTConsumer(clang::CompilerInstance &ci)
      : instance(ci), manager(ci.getSourceManager()),
        visitor(std::make_unique<SYCLVisitor>(ci, ci.getSourceManager(),
                                              this->rewriter)) {
    this->rewriter.setSourceMgr(ci.getSourceManager(), ci.getLangOpts());
  }
  // Retrieve the AST analysis result
  virtual void HandleTranslationUnit(clang::ASTContext &ctx) {
    visitor->TraverseAST(ctx);

    std::string file_name = DEFAULT_OUTPUT_NAME;
    if (OutputNameOption.size() == 1) {
      file_name = OutputNameOption.c_str();
    }

    ProgramContext program_context = visitor->get_context();

    int kernel_count = program_context.kernels.size();
    PRINT_INFO("Found {} kernels", kernel_count);
    if (kernel_count == 0) {
      return;
    }

    // create files
    std::ofstream kernel_out;
    kernel_out.open(file_name + ".c", std::ios::out);

    // Write include headers here
    std::string kernel_code;
    kernel_code.append(translator.before_kernel(program_context))
        .append(LINE_BREAK);

    // Output all kernels
    auto kernels = program_context.kernels;
    for (auto &kernel : kernels) {
      std::string kernel_str =
          translator.body_to_decl_str(program_context, kernel.second);
      kernel_code.append(kernel_str).append(LINE_BREAK);
    }
    kernel_code.append(translator.after_kernel(program_context))
        .append(LINE_BREAK);

    // write kernel code
    kernel_out << kernel_code << std::endl;

    kernel_out.close();
  }
};

class SYCLFrontendAction : public clang::PluginASTAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &ci, llvm::StringRef file) {
    return std::make_unique<SYCLASTConsumer>(ci);
  }

  bool ParseArgs(const clang::CompilerInstance &ci,
                 const std::vector<std::string> &args) {
    return true;
  }
};

int main(int argc, const char **argv) {
  llvm::Expected<clang::tooling::CommonOptionsParser> op =
      clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory,
                                                  llvm::cl::OneOrMore);
  clang::tooling::ClangTool tool(op->getCompilations(),
                                 op->getSourcePathList());
  return tool.run(
      clang::tooling::newFrontendActionFactory<SYCLFrontendAction>().get());
}