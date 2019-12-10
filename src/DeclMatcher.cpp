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
    //llvm::outs() << Decl->getDeclKindName() << "\n";
    Decl->dump();
    //llvm::outs() << "Found " << NamedDecl->getQualifiedNameAsString() << " at "
                 //<< getDeclLocation(NamedDecl->getLocation()) << "\n";
    return true;
}

bool DeclVisitor::VisitStmt(clang::Stmt *Stmt) {
    Stmt->dump();
    //llvm::outs() << "Found " << NamedDecl->getQualifiedNameAsString() << " at "
                 //<< getDeclLocation(NamedDecl->getLocation()) << "\n";
    return true;
}

bool DeclVisitor::VisitQualType(clang::QualType *QualType) {
    QualType->dump();
    //llvm::outs() << "Found " << NamedDecl->getQualifiedNameAsString() << " at "
                 //<< getDeclLocation(NamedDecl->getLocation()) << "\n";
    return true;
}

std::string DeclVisitor::getDeclLocation(clang::SourceLocation Loc) const {
    std::ostringstream OSS;
    OSS << SourceManager.getFilename(Loc).str() << ":"
        << SourceManager.getSpellingLineNumber(Loc) << ":"
        << SourceManager.getSpellingColumnNumber(Loc);
    return OSS.str();
}


DeclMatcher::DeclMatcher() {

}

bool DeclMatcher::findDecl(std::string cmd) {
    int res = clang::tooling::runToolOnCode(new DeclFindingAction, cmd);
    return res;
}

} // namspace

