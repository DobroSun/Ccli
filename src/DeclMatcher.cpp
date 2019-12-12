#include "ccli/DeclMatcher.hpp"

#include "clang/Tooling/Tooling.h"
#include <iostream>

namespace ccli {

// DeclFindingAction
std::unique_ptr<clang::ASTConsumer> DeclFindingAction::CreateASTConsumer(
                                clang::CompilerInstance &CI,
                                clang::StringRef) {
    return std::unique_ptr<clang::ASTConsumer>(new DeclFinder(CI.getSourceManager()));
}


// DeclFinder
void DeclFinder::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::SourceManager *SourceManager = &Context.getSourceManager();
    auto Decls = Context.getTranslationUnitDecl()->decls();
    for(auto &Decl: Decls) {
        const auto& FileID = SourceManager->getFileID(Decl->getLocation());
        if(FileID != SourceManager->getMainFileID()) continue;
        Visitor.TraverseDecl(Decl);
    }
}


// DeclVisitor
bool DeclVisitor::VisitDecl(clang::Decl *Decl) {
    Decl->dump();
    return true;
}


DeclMatcher::DeclMatcher() {

}

bool DeclMatcher::findDecl(std::string cmd) {
    int res = clang::tooling::runToolOnCode(new DeclFindingAction, cmd);
    return res;
}

} // namspace

