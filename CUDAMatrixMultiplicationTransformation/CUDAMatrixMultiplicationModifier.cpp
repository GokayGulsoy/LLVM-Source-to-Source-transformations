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

class ConvertTypesVisitor : public RecursiveASTVisitor<ConvertTypesVisitor> {
public:
  explicit ConvertTypesVisitor(ASTContext *Context, Rewriter &R)
      : Context(Context), R(R) {}
    
  // Visit type declarations and replace double with float.
  bool VisitTypeLoc(TypeLoc TL) {
    QualType QT = TL.getType();
    if (QT->isSpecificBuiltinType(BuiltinType::Double)) {
      // Replace the type "double" with "float".
      SourceRange ReplacementRange = TL.getSourceRange();
      StringRef ReplacementText = "float";

      // Calculate the correct replacement length to ensure adjacent words are separated.
      size_t OriginalLength = R.getRangeSize(ReplacementRange);
      size_t NewLength = ReplacementText.size();
      if (OriginalLength != NewLength) {
        // Adjust the end location to match the new length.
        SourceLocation EndLoc = ReplacementRange.getBegin().getLocWithOffset(OriginalLength - NewLength);
        ReplacementRange.setEnd(EndLoc);
      }

      // Replace and print a message indicating the type conversion.
      R.ReplaceText(ReplacementRange, ReplacementText);
    }
    return true;
  }
  
private:
  ASTContext *Context;
  Rewriter &R;

  void ReplaceWithRange(SourceLocation StartLoc, SourceLocation EndLoc, const std::string &Replacement) {
    SourceRange Range(StartLoc, EndLoc);
    R.ReplaceText(Range, Replacement);
  }
};

class ConvertCUDAMatrixASTConsumer : public ASTConsumer {
public:
  explicit ConvertCUDAMatrixASTConsumer(ASTContext *Context, Rewriter &R)
      : TypesVisitor(Context, R), TheRewriter(R) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    // Visit and transform double types to float types.
    TypesVisitor.TraverseDecl(Context.getTranslationUnitDecl());

    // Print the modified code directly from the Rewriter.
    TheRewriter.getEditBuffer(Context.getSourceManager().getMainFileID())
        .write(llvm::outs());
  }

private:
  ConvertTypesVisitor TypesVisitor;
  Rewriter &TheRewriter;
};

class ConvertCUDAMatrixFrontendAction : public ASTFrontendAction {
public:
  ConvertCUDAMatrixFrontendAction() {}

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &Compiler,
                                                 StringRef InFile) override {
    TheRewriter.setSourceMgr(Compiler.getSourceManager(), Compiler.getLangOpts());
    return std::make_unique<ConvertCUDAMatrixASTConsumer>(&Compiler.getASTContext(), TheRewriter);
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

  // Create a FrontendActionFactory for ConvertCUDAMatrixFrontendAction.
  auto Factory = newFrontendActionFactory<ConvertCUDAMatrixFrontendAction>();

  // Run the tool with the FrontendActionFactory.
  int Result = Tool.run(Factory.get());

  if (Result != 0) {
    llvm::errs() << "Error occurred while running the tool.\n";
    return Result;
  }

  return 0;
}

