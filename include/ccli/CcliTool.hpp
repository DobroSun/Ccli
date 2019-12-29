#ifndef CCLITOOL_CPP
#define CCLITOOL_CPP
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "clang/Tooling/Tooling.h"

#include "ccli/StateManager.hpp"

namespace ccli {

enum class ToolState {
    Default,
    NoErrs,
    Silent,
};


class CcliTool: public clang::tooling::ClangTool {
    std::string filename;
public:
    CcliTool(clang::tooling::CompilationDatabase &Compilations, 
             llvm::ArrayRef<std::string> SourcePath);
    StateScope Scope;

    bool execute(clang::FrontendAction *ToolAction, const std::string &code, ccli::ToolState state=ToolState::Default);

};
} // namespace
#endif
