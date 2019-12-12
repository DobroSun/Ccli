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
    //Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    Visitor.TraverseAST(Context);
}


// DeclVisitor
bool DeclVisitor::VisitDecl(clang::Decl *Decl) {
    Decl->dump();
    return true;
}

bool DeclVisitor::VisitStmt(clang::Stmt *Stmt) {
    Stmt->dump();
    return true;
}

bool DeclVisitor::VisitQualType(clang::QualType *QualType) {
    QualType->dump();
    return true;
}


DeclMatcher::DeclMatcher() {

}

bool DeclMatcher::findDecl(std::string cmd) {
    int res = clang::tooling::runToolOnCode(new DeclFindingAction, cmd);
    return res;
}

} // namspace

