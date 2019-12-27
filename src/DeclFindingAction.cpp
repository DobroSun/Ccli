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
DeclFinder::DeclFinder(clang::SourceManager &SM): Visitor(SM) {
}

void DeclFinder::HandleTranslationUnit(clang::ASTContext &Context) {
    // No need in searching for MainFile Decls.
    // This Action is supposed to be running over current line of code.
    // So just call Visitor.TraverseDecl(Decl);

    clang::SourceManager *SourceManager = &Context.getSourceManager();
    auto Decls = Context.getTranslationUnitDecl()->decls();
    for(auto &Decl: Decls) {
        const auto& FileID = SourceManager->getFileID(Decl->getLocation());
        if(FileID != SourceManager->getMainFileID()) continue;
        Visitor.TraverseDecl(Decl);
    }
}


// clang::RecursiveASTVisitor.
DeclVisitor::DeclVisitor(clang::SourceManager &SM): SourceManager(SM) {
    NoDecls = true;
}


bool DeclVisitor::VisitFunctionDecl(clang::FunctionDecl *Decl) {
    NoDecls = false;
    debug() << "Visiting FunctionDecl" << std::endl;
    debug() << NoDecls << " <- NOdecls " << std::endl;
    return true;
}
} // namspace

