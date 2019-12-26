#ifndef DUMPASTACTION_CPP
#define DUMPASTACTION_CPP
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"

namespace ccli {

class DumpASTAction : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                                    clang::CompilerInstance &CI,
                                    clang::StringRef) final;
};


class ASTVisitor: public clang::RecursiveASTVisitor<ASTVisitor> {
    clang::SourceManager &SourceManager;
public:
    ASTVisitor(clang::SourceManager &SourceManager): SourceManager(SourceManager) {}
    bool VisitDecl(clang::Decl *Decl);
    //bool VisitStmt(clang::Stmt *Stmt);
    //bool VisitQualType(clang::QualType *Type);
};


class DumpConsumer: public clang::ASTConsumer {
    ASTVisitor Visitor;
public:
    DumpConsumer(clang::SourceManager &SM): Visitor(SM) {}
    void HandleTranslationUnit(clang::ASTContext &Context) final;
};
}
#endif

