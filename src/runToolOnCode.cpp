#include "clang/AST/ASTConsumer.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

#include "ccli/runToolOnCode.hpp"
#include "ccli/Utility.hpp"


namespace ccli {

// ClangTool runs without args.
std::vector<std::string> args;

ClangTool::ClangTool() {
}

bool ClangTool::run(std::string code) {
    return clang::tooling::runToolOnCodeWithArgs(
                new clang::ento::AnalysisAction,
                code,
                args,
                "ccli.cpp",
                "main",
                std::make_shared<clang::PCHContainerOperations>(),
                std::vector<std::pair<std::string, std::string>>());
}
} // namespace
