#ifndef CCLITOOL_CPP
#define CCLITOOL_CPP
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"
#include "clang/Lex/HeaderSearch.h"

namespace ccli {

class CcliTool {
    clang::CompilerInstance *CI;
    clang::HeaderSearch *HS;

    int num_errs;
    const std::string filename;

    void init_CI();
public:
    CcliTool(clang::CompilerInstance &CI_, clang::HeaderSearch &HS);
    bool run(clang::FrontendAction *ToolAction, std::string &code);
    int get_errs() const;
};
}
#endif
