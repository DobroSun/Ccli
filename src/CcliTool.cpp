#include "clang/Tooling/Tooling.h"

#include "ccli/CcliTool.hpp"
#include "ccli/Logger.hpp"
#include "ccli/Utility.hpp"


namespace ccli {


CcliTool::CcliTool(clang::tooling::CompilationDatabase &Compilations,
                   llvm::ArrayRef<std::string> SourcePath)
                   : clang::tooling::ClangTool(Compilations, SourcePath) {
    filename = "ccli.cpp";
    toolname = "C++ Interpreter";
    args  = {
        //"ccli.cpp",
    };
}

bool CcliTool::execute(clang::FrontendAction *ToolAction, const std::string &cmd, ccli::ToolState state) {
    bool res = true;


    // FIXME:
    // Not working with runToolOnCodeWithArgs.
/*
    if(state == ToolState::Silent) {
        res = make_decorator(silent,
                    clang::tooling::runToolOnCodeWithArgs)(
                            ToolAction,
                            cmd,
                            std::vector<std::string>(),
                            filename,
                            toolname,
                            std::make_shared<clang::PCHContainerOperations>(),
                            clang::tooling::FileContentMappings()
                            );
    } else if(state == ToolState::NoErrs) {
        res = make_decorator(no_errors,
                    clang::tooling::runToolOnCodeWithArgs)(
                            ToolAction,
                            cmd,
                            std::vector<std::string>(),
                            filename,
                            toolname,
                            std::make_shared<clang::PCHContainerOperations>(),
                            clang::tooling::FileContentMappings()
                            );
*/
    //} else {


    res = clang::tooling::runToolOnCodeWithArgs(
                        ToolAction, 
                        cmd,
                        args,
                        filename,
                        toolname,
                        std::make_shared<clang::PCHContainerOperations>(),
                        clang::tooling::FileContentMappings()
                        );
    //}

    return res;
}
} // namespace
