#ifndef CCLITOOL_CPP
#define CCLITOOL_CPP
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "clang/Tooling/Tooling.h"


namespace ccli {

class CcliTool: public clang::tooling::ClangTool {
    const std::string filename;
public:
    CcliTool(clang::tooling::CompilationDatabase &Compilations, 
             llvm::ArrayRef<std::string> SourcePath)
    : clang::tooling::ClangTool(Compilations, SourcePath) {}
    bool execute(clang::FrontendAction *ToolAction, const std::string &code);
};
}
#endif
