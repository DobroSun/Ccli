#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"

#include "ccli/CcliTool.hpp"
#include "ccli/GlobalContext.hpp"
#include "ccli/exec_expr.hpp"
#include "ccli/Utility.hpp"
#include "ccli/DeclFindingAction.hpp"
#include "ccli/DumpASTAction.hpp"
#include "ccli/Logger.hpp"
#include "ccli/HeaderSearch.hpp"
#include "ccli/StateManager.hpp"

#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <string>
#include <csignal>
#include <fstream>
#include <cstdio>
#include <cstring>




static llvm::cl::extrahelp
    CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);

llvm::cl::OptionCategory CcliCategory("ccli options");
static char CcliUsage[] = "Usage: ccli [option]";

/*
void collect_headers() {
    std::vector<const char*> headers;
    std::vector<std::string> sheaders = get_headers();
    make_headers(sheaders, headers);

    debug() << "Processed headers: " << std::endl;
    print(headers);
    debug() << "********* Headers collected *********" << std::endl;
}
*/


std::string welcome() {
    std::string welcome = "Hello world> ";
    return welcome;
}


int main(int argc, const char **argv) {
    llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);

    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();

    // Have to parse command line arguments
    // And add help... and so forth.
    //clang::tooling::CommonOptionsParser option(argc, argv, CcliCategory, CcliUsage);

    clang::CompilerInstance CI(std::make_shared<clang::PCHContainerOperations>());

    clang::tooling::CommonOptionsParser ToolOptions(argc, argv, CcliCategory);
    ccli::CcliTool Tool(ToolOptions.getCompilations(),
                        ToolOptions.getSourcePathList());

    //collect_headers();


    ccli::GlobalContext GlobalContext;
    ccli::StateScope Scope;
    ccli::StateManager StateManager(GlobalContext);

    // Frontend Actions that will be processed
    // With CcliTool.
    //std::unique_ptr<ccli::DeclFindingAction> FindAct(new ccli::DeclFindingAction);
    std::unique_ptr<ccli::DumpASTAction> DumpAct(new ccli::DumpASTAction);
    std::unique_ptr<clang::ento::AnalysisAction> AnalysisAct(new clang::ento::AnalysisAction);
    std::shared_ptr<clang::SyntaxOnlyAction> SyntaxOnlyAct(new clang::SyntaxOnlyAction);


    // Handles Ctrl-C interruption
    struct sigaction act;
    act.sa_handler = [](int sig){
                        std::cout << "\n";
                        rl_on_new_line();
                        rl_replace_line("", 0);
                        rl_redisplay();
                     };
    sigaction(SIGINT, &act, NULL);
    rl_bind_key('\t', rl_insert);

    while(1) {
        const char *cmd = readline(welcome().c_str());

        // Handles C-d and whitespace
        if(cmd == NULL) {std::cout << "\n"; exit(0);}
        if(!std::strcmp(cmd, "")) continue;

        add_history(cmd);

        // FIXME:
        // Make deep copies of Actions and pass them to function.
        //Tool.execute(FindAct .get(), context_string);
        //Tool.execute(AnalysisAct.get(), context_string);
        //Tool.execute(SyntaxOnlyAct.get(), "int a;");


        //Tool.execute(new clang::SyntaxOnlyAction, "int a;");
        //Tool.execute(new clang::ento::AnalysisAction, context_string);
        //Tool.execute(new ccli::DeclFindingAction(&Scope), cmd, ccli::ToolState::NoErrs);



        // TODO:
        // Make adding commands logic!!!
        StateManager.change_state(Scope);
        GlobalContext.add_command(cmd);

        std::string context_string = GlobalContext.get_context();
        debug() << context_string << " <- context_string" << std::endl;


#ifdef DEBUG
        Tool.execute(new ccli::DumpASTAction, context_string);
#endif








        std::string result = exec_expr(cmd);
        std::cout << result;
    }
    return 0;
}
