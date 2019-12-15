#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Lex/HeaderSearch.h"

#include "ccli/CcliTool.hpp"
#include "ccli/GlobalContext.hpp"
#include "ccli/exec_expr.hpp"
#include "ccli/Utility.hpp"
#include "ccli/DeclFindingAction.hpp"
#include "ccli/Logger.hpp"

#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <string>
#include <csignal>
#include <fstream>
#include <cstdio>
#include <cstring>


#define GET_HEADERS_CMD "gcc -v -E -xc - < /dev/null 2>&1 | sed -ne '/starts here/,/End of/p' | grep -v '#include' | grep -v 'End of search list'"


static llvm::cl::extrahelp
    CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);

llvm::cl::OptionCategory CcliCategory("ccli options");
static char CcliUsage[] = "Usage: ccli [option]";


void Init_CI(clang::CompilerInstance &CI) {
    // Initialize DiagnosticsEngine.
    CI.createDiagnostics();
    clang::DiagnosticsEngine &DiagnosticsEngine = CI.getDiagnostics();

    // Initialize llvm TargetInfo.
    auto TO = std::make_shared<clang::TargetOptions>();
    TO->Triple = llvm::sys::getDefaultTargetTriple();
    clang::TargetInfo *TI = clang::TargetInfo::CreateTargetInfo(CI.getDiagnostics(), TO);
    CI.setTarget(TI);

    // Initialize File, Source Managers. 
    CI.createFileManager();
    clang::FileManager &FileManager = CI.getFileManager();
    CI.createSourceManager(FileManager);
    clang::SourceManager &SourceManager = CI.getSourceManager();

    // Initialize LangOptions.
    clang::LangOptions LangOpts;
    debug() << "******** CompilerInstance is initialized ********" << std::endl;
}


void Init_HS(clang::CompilerInstance &CI, clang::HeaderSearch *HS) {
    std::vector<std::string> headers = get_splitted_exec(GET_HEADERS_CMD);


    clang::FileManager &FileManager = CI.getFileManager();
    clang::SourceManager &SourceManager = CI.getSourceManager();
    clang::DiagnosticsEngine &DiagnosticsEngine = CI.getDiagnostics();
    clang::LangOptions LangOpts = CI.getLangOpts();
    clang::TargetInfo &TargetInfo = CI.getTarget();

    // Initialize HeaderSearch.
    std::shared_ptr<clang::HeaderSearchOptions> HeaderSearchOptions =
        std::make_shared<clang::HeaderSearchOptions>();

    debug() << "Start of print" << std::endl;
    //print(headers);
    debug() << "End of print" << std::endl;

    //HeaderSearchOptions->UseStandardSystemIncludes = 1;

/*
    std::vector<clang::DirectoryLookup> Dirs;
    
    for(std::string path: headers) {
        clang::DirectoryLookup lookup = clang::DirectoryLookup(
                FileManager.getDirectory(path),
                clang::SrcMgr::CharacteristicKind::C_System,
                false);

        debug() << path << " <- Pushing to vector of dirs" << std::endl;
        Dirs.push_back(lookup);
    }
*/
    HS = new clang::HeaderSearch(HeaderSearchOptions, SourceManager,
       DiagnosticsEngine, LangOpts, &TargetInfo);
    //HS->SetSearchPaths(Dirs, 0, 0, true);
    debug() << "******** HeaderSearch is initialized ********" << std::endl;
}


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
    clang::tooling::CommonOptionsParser option(argc, argv, CcliCategory, CcliUsage);

    clang::CompilerInstance CI(std::make_shared<clang::PCHContainerOperations>());
    std::unique_ptr<clang::HeaderSearch> HS;


    Init_CI(CI);
    Init_HS(CI, HS.get());


    ccli::CcliTool Tool(CI, *(HS.get()));
    ccli::GlobalContext GlobalContext;

    // Frontend Actions that will be processed
    // With CcliTool.
    std::unique_ptr<ccli::DeclFindingAction> Finding_Act(new ccli::DeclFindingAction);
    std::unique_ptr<clang::ento::AnalysisAction> Analysis_Act(new clang::ento::AnalysisAction);


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

    while (1) {
        const char *cmd = readline(welcome().c_str());

        // Handles C-d and whitespace
        if (cmd == NULL) {std::cout << "\n"; exit(0);}
        if (!std::strcmp(cmd, "")) continue;

        add_history(cmd);
        GlobalContext.add_command(cmd);
        std::string context_string = GlobalContext.get_context();


        Tool.run(Analysis_Act.get(), context_string);

        //Tool.run(Analysis_Act, compiled_str);


        std::string result = exec_expr(cmd);
        std::cout << result;
    }
    return 0;
}
