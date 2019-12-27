#include "clang/Tooling/Tooling.h"

#include "ccli/CcliTool.hpp"
#include "ccli/Logger.hpp"
#include "ccli/Utility.hpp"


namespace ccli {


bool CcliTool::execute(clang::FrontendAction *ToolAction, const std::string &cmd, ccli::ExecutionState state) {
/*
    //mapVirtualFile("ccli.cpp", cmd);

    clearArgumentsAdjusters();
    appendArgumentsAdjuster(clang::tooling::getClangStripOutputAdjuster());
    appendArgumentsAdjuster(clang::tooling::getClangStripDependencyFileAdjuster());

    //appendArgumentsAdjuster("-I/usr/include/c++

    bool res = run(ToolAction);

*/

    bool res;
    if(state == Silent) {
        res = make_decorator(silent,
                    clang::tooling::runToolOnCode)(
                            ToolAction,
                            cmd,
                            "ccli.cpp",
                            std::make_shared<clang::PCHContainerOperations>()
                            );
    }
    else
        res = clang::tooling::runToolOnCode(ToolAction, cmd);
    return res;
}
} // namespace
