#ifndef DECLFINDINGACTION_CPP
#define DECLFINDINGACTION_CPP
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"


#include <string>

namespace ccli {

class DeclFindingAction: public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                                    clang::CompilerInstance &CI,
                                    clang::StringRef) final;
};


class DeclVisitor: public clang::RecursiveASTVisitor<DeclVisitor> {
    clang::SourceManager &SourceManager;
    bool NoDecls;
public:
    DeclVisitor(clang::SourceManager &SM);
    bool VisitFunctionDecl(clang::FunctionDecl *Decl);
};


class DeclFinder: public clang::ASTConsumer {
    DeclVisitor Visitor;
public:
    DeclFinder(clang::SourceManager &SM);
    void HandleTranslationUnit(clang::ASTContext &Context) final;
};

} // namespace
#endif
