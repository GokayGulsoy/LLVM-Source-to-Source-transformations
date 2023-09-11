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

class ConvertDoubleToFloatVisitor : public RecursiveASTVisitor<ConvertDoubleToFloatVisitor> {
public:
  explicit ConvertDoubleToFloatVisitor(ASTContext *Context, Rewriter &R)
      : Context(Context), R(R) {}

  // Visit type declarations and replace double with float.
  bool VisitTypeLoc(TypeLoc TL) {
    if (TL.getType().getAsString() == "double") {
      // Replace the type "double" with "float".
      SourceRange ReplacementRange = TL.getLocalSourceRange();
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
      // I have put the following print statements for debugging
     /*llvm::outs() << "Converted 'double' to 'float' in type declaration.\n";*/
      
    }
    return true;
  }

  // Visit variable declarations and replace double with float.
  bool VisitVarDecl(VarDecl *VD) {
    if (VD->getType().getAsString() == "double") {
      // Replace the type "double" with "float".
      SourceRange ReplacementRange = VD->getTypeSourceInfo()->getTypeLoc().getLocalSourceRange();
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
      // I have put the following print statement for debugging
      /*llvm::outs() << "Converted 'double' to 'float' in variable declaration: " << VD->getNameAsString() << "\n";*/
    }
    return true;
  }

private:
  ASTContext *Context;
  Rewriter &R;
};

class ConvertDoubleToFloatASTConsumer : public ASTConsumer {
public:
  explicit ConvertDoubleToFloatASTConsumer(ASTContext *Context, Rewriter &R)
      : Visitor(Context, R), TheRewriter(R) {}

  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());

    // Get the modified code from the Rewriter and print it to llvm::outs().
    const RewriteBuffer *RewriteBuf = TheRewriter.getRewriteBufferFor(Context.getSourceManager().getMainFileID());
    if (RewriteBuf) {
      /*llvm::outs() << "Modified Code:\n";*/
      for (char C : *RewriteBuf)
        llvm::outs().write(&C, 1);
    }
  }

private:
  ConvertDoubleToFloatVisitor Visitor;
  Rewriter &TheRewriter;
};

class ConvertDoubleToFloatFrontendAction : public ASTFrontendAction {
public:
  ConvertDoubleToFloatFrontendAction() {}

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &Compiler,
                                                 StringRef InFile) override {
    TheRewriter.setSourceMgr(Compiler.getSourceManager(), Compiler.getLangOpts());
    return std::make_unique<ConvertDoubleToFloatASTConsumer>(&Compiler.getASTContext(), TheRewriter);
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

  // Create a FrontendActionFactory for ConvertDoubleToFloatFrontendAction.
  auto Factory = newFrontendActionFactory<ConvertDoubleToFloatFrontendAction>();

  // Run the tool with the FrontendActionFactory.
  int Result = Tool.run(Factory.get());

  if (Result != 0) {
    llvm::errs() << "Error occurred while running the tool.\n";
    return Result;
  }

  return 0;
}
