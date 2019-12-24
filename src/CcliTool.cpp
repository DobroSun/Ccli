#include "clang/Tooling/Tooling.h"

#include "ccli/Logger.hpp"
#include "ccli/CcliTool.hpp"


namespace ccli {


bool CcliTool::run(std::unique_ptr<clang::tooling::FrontendActionFactory> ToolAction, std::string &cmd) {
    mapVirtualFile("ccli.cpp", "#include <string>");

    clang::tooling::ArgumentsAdjuster adj = clang::tooling::getClangSyntaxOnlyAdjuster();

    llvm::ArrayRef<std::pair<std::unique_ptr<clang::tooling::FrontendActionFactory>, clang::tooling::ArgumentsAdjuster>> args;
    std::pair<std::unique_ptr<clang::tooling::FrontendActionFactory>, clang::tooling::ArgumentsAdjuster> pr = std::make_pair(ToolAction, adj);


    llvm::Error res = execute(args);

    
    return (res)? 1: 0;
}

} // namespace
