#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp("\nMore help text...\n");

// ...
class RenameFunctionVisitor : public RecursiveASTVisitor<RenameFunctionVisitor> {
public:
  explicit RenameFunctionVisitor(ASTContext *Context, Rewriter &R)
      : Context(Context), R(R) {}

  // Visit function declarations and rename walk() to run().
  bool VisitFunctionDecl(FunctionDecl *FD) {
    if (FD->getNameInfo().getName().getAsString() == "walk") {
      // Replace the function name "walk" with "run".
      SourceLocation StartLoc = FD->getLocation();
      SourceLocation EndLoc = StartLoc.getLocWithOffset(strlen("walk") - 1);
      SourceRange ReplacementRange(StartLoc, EndLoc);
      StringRef ReplacementText = "run";

      // Replace and print a message indicating the renaming and the new function name.
      R.ReplaceText(ReplacementRange, ReplacementText);
      llvm::outs() << "Renamed function from 'walk' to 'run': " << FD->getNameAsString() << "\n";
    }
    return true;
  }

  // Visit member function call expressions and rename walk() to run().
  bool VisitCXXMemberCallExpr(CXXMemberCallExpr *CE) {
    if (CE->getMethodDecl() && CE->getMethodDecl()->getNameInfo().getName().getAsString() == "walk") {
      // Replace the member function call "walk" with "run".
      SourceLocation StartLoc = CE->getExprLoc();
      SourceLocation EndLoc = StartLoc.getLocWithOffset(strlen("walk") - 1);
      SourceRange ReplacementRange(StartLoc, EndLoc);
      StringRef ReplacementText = "run";
      // Replace and print a message indicating the renaming and the new member function name.
      R.ReplaceText(ReplacementRange, ReplacementText);
      llvm::outs() << "Renamed member function call from 'walk' to 'run'\n";
    }
    return true;
  }

private:
  ASTContext *Context;
  Rewriter &R;
};

// ...

// An ASTConsumer that uses the RenameFunctionVisitor.
class RenameFunctionASTConsumer : public ASTConsumer {
public:
  explicit RenameFunctionASTConsumer(ASTContext *Context, Rewriter &R)
      : Visitor(Context, R), TheRewriter(R) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());

    // Get the modified code from the Rewriter and print it to llvm::outs().
    const RewriteBuffer *RewriteBuf = TheRewriter.getRewriteBufferFor(Context.getSourceManager().getMainFileID());
    if (RewriteBuf) {
      llvm::outs() << "Modified Code:\n";
      for (char C : *RewriteBuf)
        llvm::outs().write(&C, 1);
    }
  }

private:
  RenameFunctionVisitor Visitor;
  Rewriter &TheRewriter;
};

// FrontendActionFactory for the tool.
class RenameFunctionFrontendAction : public ASTFrontendAction {
public:
  RenameFunctionFrontendAction() {}

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &Compiler,
                                                 StringRef InFile) override {
    TheRewriter.setSourceMgr(Compiler.getSourceManager(), Compiler.getLangOpts());
    return std::make_unique<RenameFunctionASTConsumer>(&Compiler.getASTContext(), TheRewriter);
  }

private:
  Rewriter TheRewriter;
};

int main(int argc, const char **argv) {
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }

  CommonOptionsParser &op = ExpectedParser.get();
  ClangTool Tool(op.getCompilations(), op.getSourcePathList());

  // Create a FrontendActionFactory for RenameFunctionFrontendAction.
  auto Factory = newFrontendActionFactory<RenameFunctionFrontendAction>();

  // Run the tool with the FrontendActionFactory.
  int Result = Tool.run(Factory.get());

  if (Result != 0) {
    llvm::errs() << "Error occurred while running the tool.\n";
    return Result;
  }

  return 0;
}

