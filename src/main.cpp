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
#include "ccli/FindActionAdjuster.hpp"

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
    //clang::SourceManager &SourceManager = CI.getSourceManager();

    // Initialize LangOptions.
    clang::LangOptions &LangOpts = CI.getLangOpts();

    // Initialize Preprocessor.
    CI.createPreprocessor(clang::TU_Module);
    clang::Preprocessor &Preprocessor = CI.getPreprocessor();

    // Initialize HeaderSearch
    // clang::HeaderSearch &HS = Preprocessor.getHeaderSearchInfo();



    std::vector<const char*> headers;
    std::vector<std::string> sheaders = get_headers();
    make_headers(sheaders, headers);

    debug() << "Processed headers: " << std::endl;
    print(headers);


    llvm::ArrayRef<const char*> args(headers);
    // Initialize Invocation
    std::shared_ptr<clang::CompilerInvocation> CInv(new clang::CompilerInvocation);
    CInv->CreateFromArgs(*(CInv.get()), args.begin(), args.end(), DiagnosticsEngine);
    CI.setInvocation(CInv);

    debug() << "******** CompilerInstance is initialized ********" << std::endl;
}

/*
void Init_CcliTool(ccli::CcliTool &Tool) {
    Tool.clearArgumentsAdjusters();
    Tool.appendArgumentsAdjuster(clang::tooling::getClangStripOutputAdjuster());
    Tool.appendArgumentsAdjuster(clang::tooling::getClangStripDependencyFileAdjuster());

    //Tool.appendArgumentsAdjuster("
}
*/

std::string welcome() {
    std::string welcome = "Hello world> ";
    return welcome;
}

int add(int a, double b, std::string s) {
    return a + b;
}


int main(int argc, const char **argv) {
    auto f = make_decorator(silent, add)(1, 3.0, "loo");
    //std::cout << f() << std::endl;
    std::cout << f << std::endl;

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


    Init_CI(CI);
    //Init_CcliTool(Tool);




    ccli::GlobalContext GlobalContext;
    ccli::FindActionAdjuster FindActionAdjuster;

    // Frontend Actions that will be processed
    // With CcliTool.
    std::unique_ptr<ccli::DeclFindingAction> FindAct(new ccli::DeclFindingAction);
    std::unique_ptr<ccli::DumpASTAction> DumpAct(new ccli::DumpASTAction);
    std::unique_ptr<clang::ento::AnalysisAction> AnalysisAct(new clang::ento::AnalysisAction);
    std::unique_ptr<clang::SyntaxOnlyAction> SyntaxOnlyAct(new clang::SyntaxOnlyAction);


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


        GlobalContext.add_command(cmd);
        std::string context_string = GlobalContext.get_context();
        debug() << context_string << " <- context_string" << std::endl;


        // FIXME:
        // Make deep copies of Actions and pass them to function.
        //Tool.execute(new clang::SyntaxOnlyAction, context_string);
        //Tool.execute(new clang::ento::AnalysisAction, context_string);
#ifdef DEBUG
        Tool.execute(new ccli::DumpASTAction, context_string);
#endif
        Tool.execute(new ccli::DeclFindingAction, cmd, ccli::Silent);




        //Tool.execute(SyntaxOnlyAct.get(), context_string);
        //Tool.execute(FindAct.get(), context_string);
        //Tool.execute(Analysis_Act, context_string);


        std::string result = exec_expr(cmd);
        std::cout << result;
    }
    return 0;
}
