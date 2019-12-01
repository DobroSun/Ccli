#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include "ccli/exec_expr.hpp"
#include <string>

using namespace clang::tooling;
using namespace llvm;
/*
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

static cl::extrahelp MoreHelp("\nMore help text...\n");
*/
std::string exec_expr(std::string cmd){
    return "";
}
bool analyze(std::string cmd) {
/*
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);

    ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

    return Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
*/
    return clang::tooling::runToolOnCode(new clang::SyntaxOnlyAction, cmd);
}

