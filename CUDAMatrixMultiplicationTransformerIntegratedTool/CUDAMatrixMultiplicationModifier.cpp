#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/StmtCXX.h"
#include "clang/Basic/SourceLocation.h"
#include "string.h"
#include <string>
#include <iostream>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;
using namespace std;

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

cl::opt<std::string>
    ThreadsValue("threads",
                 cl::desc("Specify the value to replace THREADS with"),
                 cl::value_desc("value"), cl::init("32"));
                 
cl::opt<std::string>
     ThreadReductionRatio("reduction-ratio",
     	             	   cl::desc("Specify the value for reduction for number of threads"),			                 
                           cl::init("0"));
cl::opt<bool>
    ConvertDoubleToFloat("convert-double-to-float",
                         cl::desc("Convert double variables to float."),
                         cl::init(false));

class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
  explicit MyASTVisitor(ASTContext *Context, Rewriter &R)
      : Context(Context), R(R) {}

  // Visit type declarations and replace double with float.
  bool VisitTypeLoc(TypeLoc TL) {
    if (ConvertDoubleToFloat) {
      QualType QT = TL.getType();
      if (QT->isSpecificBuiltinType(BuiltinType::Double)) {
        // Replace the type "double" with "float".
        SourceRange ReplacementRange = TL.getSourceRange();
        StringRef ReplacementText = "float";
        size_t OriginalLength = R.getRangeSize(ReplacementRange);
        size_t NewLength = ReplacementText.size();
        if (OriginalLength != NewLength) {
          SourceLocation EndLoc = ReplacementRange.getBegin().getLocWithOffset(
              OriginalLength - NewLength);
          ReplacementRange.setEnd(EndLoc);
        }
        R.ReplaceText(ReplacementRange, ReplacementText);
      }
    }
    return true;
  }
  
  // Visit variable declarations and change the threads_per_block value.
  bool VisitVarDecl(VarDecl *VD) {
    if (VD->getNameAsString() == "threadsPerBlock") {
      // Replace the THREADS value with the user-specified value.
      if (VD->hasInit()) {
        Expr *InitExpr = VD->getInit();
        SourceRange ReplacementRange = InitExpr->getSourceRange();
        int ThreadValueint = stoi(ThreadsValue);
 	int reductionRatioint = stoi(ThreadReductionRatio); 
 	 
 	int reducedThreadValue = ThreadValueint - (ThreadValueint * reductionRatioint) / 100;
        string NewThreadsValue = to_string(reducedThreadValue);
        StringRef ReplacementText = NewThreadsValue;
        R.ReplaceText(ReplacementRange, ReplacementText);
      }
    }
    return true;
  }

  // Visit call expressions and change the second parameter in CUDA kernel calls.
  bool VisitCallExpr(CallExpr *CE) {
    if (isCUDAKernelCall(CE)) {
      SourceLocation BeginLoc = CE->getExprLoc();
      SourceLocation EndLoc = CE->getRParenLoc();

      if (!BeginLoc.isInvalid() && !EndLoc.isInvalid()) {
        StringRef CallText = Lexer::getSourceText(CharSourceRange::getCharRange(BeginLoc, EndLoc),
                                                  Context->getSourceManager(), Context->getLangOpts());

        if (CallText.startswith("<<<")) {
 	  int ThreadValueint = stoi(ThreadsValue);
 	  int reductionRatioint = stoi(ThreadReductionRatio); 
 	 
 	  int reducedThreadValue = ThreadValueint - (ThreadValueint * reductionRatioint) / 100;
 	  string NewThreadsValue = to_string(reducedThreadValue);
 	  //llvm::outs() << "Found CUDA Kernel Call: " << CallText << "\n";
          // Replace the CUDA kernel launch with the value from the command line
          std::string ReplacementText = "<<<blocks, " + NewThreadsValue + "";
          size_t OriginalLength = CallText.size();
          size_t NewLength = ReplacementText.size();
          SourceRange ReplacementRange = SourceRange(BeginLoc, EndLoc);

          if (OriginalLength != NewLength) {
            SourceLocation EndLoc = ReplacementRange.getBegin().getLocWithOffset(NewLength);
            ReplacementRange.setEnd(EndLoc);
          }
          R.ReplaceText(ReplacementRange, ReplacementText);
        }
      }
    }
    return true;
  }

private:
  ASTContext *Context;
  Rewriter &R;

  bool isCUDAKernelCall(CallExpr *CE) {
    if (CE->getDirectCallee()) {
      // Check if the function is called with "<<<" and ">>>".
      SourceLocation BeginLoc = CE->getExprLoc();
      SourceLocation EndLoc = CE->getRParenLoc();

      if (!BeginLoc.isInvalid() && !EndLoc.isInvalid()) {
        StringRef CallText = Lexer::getSourceText(CharSourceRange::getCharRange(BeginLoc, EndLoc),
                                                  Context->getSourceManager(), Context->getLangOpts());

	//llvm::outs() << "Any Call Expression: " << CallText << "\n";
        if (CallText.startswith("<<<")) {
          //printf("We are inside this if");
          //llvm::outs() << "Found CUDA Kernel Call: " << CallText << "\n";
          return true;
        }
      }
    }
    return false;
  }
};

class MyASTConsumer : public ASTConsumer {
public:
  explicit MyASTConsumer(ASTContext *Context, Rewriter &R)
      : Visitor(Context, R), TheRewriter(R) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    TheRewriter.getEditBuffer(Context.getSourceManager().getMainFileID())
        .write(llvm::outs());
  }

private:
  MyASTVisitor Visitor;
  Rewriter &TheRewriter;
};

class MyFrontendAction : public ASTFrontendAction {
public:
  MyFrontendAction() {}

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &Compiler,
                                                 StringRef InFile) override {
    TheRewriter.setSourceMgr(Compiler.getSourceManager(),
                             Compiler.getLangOpts());
    return std::make_unique<MyASTConsumer>(&Compiler.getASTContext(),
                                           TheRewriter);
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

  auto Factory = newFrontendActionFactory<MyFrontendAction>();
  int Result = Tool.run(Factory.get());

  if (Result != 0) {
    llvm::errs() << "Error occurred while running the tool.\n";
    return Result;
  }

  return 0;
}

