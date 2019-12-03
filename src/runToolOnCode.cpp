#include "clang/AST/ASTConsumer.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

#include "ccli/runToolOnCode.hpp"

// ClangTool runs without args.
std::vector<std::string> args;

namespace ccli {

ClangTool::ClangTool() {
    std::string file_buffer = "";
    std::string file_name = "ccli.cpp";

    std::vector<std::pair<std::string, std::string>> VirtualFiles;
}

void ClangTool::push_to_vf(std::string cmd) {
    file_buffer += "\n" + cmd;
    if(VirtualFiles.empty()) {
        VirtualFiles.push_back(std::make_pair(file_name, file_buffer));
    } else {
        VirtualFiles.pop_back();
        VirtualFiles.push_back(std::make_pair(file_name, file_buffer));
    }
}

bool ClangTool::run(std::string cmd) {
    push_to_vf(cmd);

    return clang::tooling::runToolOnCodeWithArgs(
                new clang::ento::AnalysisAction,
                "",
                args,
                "ccli.cpp",
                "C++ interpreter",
                std::make_shared<clang::PCHContainerOperations>(),
                VirtualFiles);
}
}
