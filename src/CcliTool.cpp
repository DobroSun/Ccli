#include "clang/Tooling/Tooling.h"

#include "ccli/CcliTool.hpp"
#include "ccli/Logger.hpp"
#include "ccli/Utility.hpp"


namespace ccli {


CcliTool::CcliTool(clang::tooling::CompilationDatabase &Compilations,
                   llvm::ArrayRef<std::string> SourcePath)
                   : clang::tooling::ClangTool(Compilations, SourcePath) {
    filename = "ccli.cpp";
}
bool CcliTool::execute(clang::FrontendAction *ToolAction, const std::string &cmd, ccli::ToolState state) {
    bool res;
    if(state == ToolState::Silent) {
        res = make_decorator(silent,
                    clang::tooling::runToolOnCode)(
                            ToolAction,
                            cmd,
                            filename,
                            std::make_shared<clang::PCHContainerOperations>()
                            );
    } else if(state == ToolState::NoErrs) {
        res = make_decorator(no_errors,
                    clang::tooling::runToolOnCode)(
                            ToolAction,
                            cmd,
                            filename,
                            std::make_shared<clang::PCHContainerOperations>()
                            );
    } else {
        res = clang::tooling::runToolOnCode(ToolAction, cmd);
    }
    return res;
}
} // namespace
