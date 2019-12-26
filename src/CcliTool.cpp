#include "clang/Tooling/Tooling.h"

#include "ccli/CcliTool.hpp"
#include "ccli/Logger.hpp"


namespace ccli {


bool CcliTool::execute(clang::FrontendAction *ToolAction, const std::string &cmd) {
/*
    //mapVirtualFile("ccli.cpp", cmd);

    clearArgumentsAdjusters();
    appendArgumentsAdjuster(clang::tooling::getClangStripOutputAdjuster());
    appendArgumentsAdjuster(clang::tooling::getClangStripDependencyFileAdjuster());

    //appendArgumentsAdjuster("-I/usr/include/c++

    bool res = run(ToolAction);

*/
    bool res = clang::tooling::runToolOnCode(ToolAction, cmd);

    if(res) 
        debug() << "No errors in AST" << std::endl;
    return res;
}
} // namespace
