#ifndef CCLI_RUNTOOLONCODE
#define CCLI_RUNTOOLONCODE
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"

namespace ccli {

class CcliTool {
    clang::CompilerInstance *CI;

    int num_errs;
    const std::string filename;

    void init_CI();
public:
    CcliTool(clang::CompilerInstance &CI_);
    bool run(clang::FrontendAction *ToolAction);
    //bool run(std::string cmd);
    int get_errs() const;
};
}
#endif
