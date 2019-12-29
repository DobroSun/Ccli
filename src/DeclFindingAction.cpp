#include "ccli/DeclFindingAction.hpp"
#include "ccli/Logger.hpp"


namespace ccli {
class DeclFinder;


// clang::ASTFrontendAction.
DeclFindingAction::DeclFindingAction(StateScope &Scope_) {
    Scope = Scope_;
}


std::unique_ptr<clang::ASTConsumer> DeclFindingAction::CreateASTConsumer(
                                clang::CompilerInstance &CI,
                                clang::StringRef) {
    return std::unique_ptr<clang::ASTConsumer>(new DeclFinder(CI.getSourceManager(), Scope));
}


// clang::ASTConsumer.
DeclFinder::DeclFinder(clang::SourceManager &SM, StateScope &Scope_): Visitor(SM, Scope_) {
    Scope = Scope_;
}

void DeclFinder::HandleTranslationUnit(clang::ASTContext &Context) {
    Visitor.TraverseAST(Context);
}


// clang::RecursiveASTVisitor.
DeclVisitor::DeclVisitor(clang::SourceManager &SM, StateScope &Scope_): SourceManager(SM) {
    Scope = Scope_;
}

bool DeclVisitor::VisitFunctionDecl(clang::FunctionDecl *Decl) {
    Scope.is_decl = true;
    debug() << "Changed state of 'Decls'" << std::endl;
    return true;
}
} // namspace

