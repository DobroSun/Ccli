#ifndef CCLITOOL_CPP
#define CCLITOOL_CPP
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "clang/Tooling/StandaloneExecution.h"


namespace ccli {

class CcliTool: public clang::tooling::StandaloneToolExecutor{
    const std::string filename;
public:
    CcliTool(clang::tooling::CompilationDatabase &Compilations, 
             llvm::ArrayRef<std::string> SourcePath)
    : clang::tooling::StandaloneToolExecutor(Compilations, SourcePath) {}
    bool run(std::unique_ptr<clang::tooling::FrontendActionFactory> ToolAction, std::string &code);
};
}
#endif
