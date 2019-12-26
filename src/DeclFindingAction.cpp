#include "ccli/DeclFindingAction.hpp"
#include "ccli/Logger.hpp"


namespace ccli {

// clang::ASTFrontendAction.
std::unique_ptr<clang::ASTConsumer> DeclFindingAction::CreateASTConsumer(
                                clang::CompilerInstance &CI,
                                clang::StringRef) {
    return std::unique_ptr<clang::ASTConsumer>(new DeclFinder(CI.getSourceManager()));
}


// clang::ASTConsumer.
void DeclFinder::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::SourceManager *SourceManager = &Context.getSourceManager();
    auto Decls = Context.getTranslationUnitDecl()->decls();
    for(auto &Decl: Decls) {
        const auto& FileID = SourceManager->getFileID(Decl->getLocation());
        if(FileID != SourceManager->getMainFileID()) continue;
        Visitor.TraverseDecl(Decl);
    }

}


// clang::RecursiveASTVisitor.
bool DeclVisitor::VisitFunctionDecl(clang::FunctionDecl *Decl) {
    debug() << "Visiting FunctionDecl" << std::endl;
    return true;
}
} // namspace

