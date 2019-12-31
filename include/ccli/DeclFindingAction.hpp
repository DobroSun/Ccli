#ifndef DECLFINDINGACTION_CPP
#define DECLFINDINGACTION_CPP
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"

#include "ccli/StateManager.hpp"

#include <string>

namespace ccli {


class DeclFindingAction: public clang::ASTFrontendAction {
    StateScope *Scope;
public:
    DeclFindingAction(StateScope *Scope_);
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                                    clang::CompilerInstance &CI,
                                    clang::StringRef) final;
};


class DeclVisitor: public clang::RecursiveASTVisitor<DeclVisitor> {
    clang::SourceManager &SourceManager;
    StateScope *Scope;
public:
    DeclVisitor(clang::SourceManager &SM, StateScope *Scope);
    bool VisitFunctionDecl(clang::FunctionDecl *Decl);
    bool VisitVarDecl(clang::VarDecl *Decl);
};


class DeclFinder: public clang::ASTConsumer {
    StateScope *Scope;
    DeclVisitor Visitor;
public:
    DeclFinder(clang::SourceManager &SM, StateScope *Scope_);
    void HandleTranslationUnit(clang::ASTContext &Context) final;
};

} // namespace
#endif
