#include "ccli/exec_expr.hpp"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/CodeGen/ObjectFilePCHContainerOperations.h"
#include "clang/Driver/Options.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Frontend/FixItRewriter.h"
#include "clang/Rewrite/Frontend/FrontendActions.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"


using namespace clang::tooling;

std::string exec_expr(std::string cmd) {
    return cmd;
}

bool analyze(std::string cmd) {
    ClangTool Tool();

    std::unique_ptr<FrontendActionFactory> FrontendFactory;
    FrontendFactory = newFrontendActionFactory<clang::ento::AnalysisAction>();
    return Tool.run(FrontendFactory.get());
}
