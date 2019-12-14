#ifndef DECLMATCHER_CPP
#define DECLMATCHER_CPP
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <sstream>

namespace ccli {

class DeclFindingAction: public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                                    clang::CompilerInstance &CI,
                                    clang::StringRef) final;
};


class DeclVisitor: public clang::RecursiveASTVisitor<DeclVisitor> {
    clang::SourceManager &SourceManager;
public:
    DeclVisitor(clang::SourceManager &SourceManager): SourceManager(SourceManager) {}
    bool VisitDecl(clang::Decl *Decl);
private:
};


class DeclFinder: public clang::ASTConsumer {
    DeclVisitor Visitor;
public:
    DeclFinder(clang::SourceManager &SM): Visitor(SM) {}
    void HandleTranslationUnit(clang::ASTContext &Context) final;
};


class DeclMatcher {
    
public:
    DeclMatcher();
    bool findDecl(std::string cmd);
    //std::string getAST(std::string cmd);
};
}
#endif
