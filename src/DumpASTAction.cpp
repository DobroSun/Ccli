#include "ccli/DumpASTAction.hpp"
#include "ccli/Logger.hpp"

namespace ccli {

static bool is_in_main(clang::SourceManager &SM, clang::Decl *Decl) {
    return SM.isInMainFile(Decl->getBeginLoc());
}


// clang::ASTFrontendAction.
std::unique_ptr<clang::ASTConsumer> DumpASTAction::CreateASTConsumer(
                                clang::CompilerInstance &CI,
                                clang::StringRef) {
    return std::unique_ptr<clang::ASTConsumer>(new DumpConsumer(CI.getSourceManager()));
}


// clang::ASTConsumer.
void DumpConsumer::HandleTranslationUnit(clang::ASTContext &Context) {
   Visitor.TraverseAST(Context);
}


// clang::RecursiveASTVisitor.
bool ASTVisitor::VisitDecl(clang::Decl *Decl) {
    if(is_in_main(SourceManager, Decl))
        Decl->dump();
    return true;
}
} // namspace
