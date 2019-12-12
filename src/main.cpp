#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Parse/ParseAST.h"

#include "ccli/runToolOnCode.hpp"
#include "ccli/GlobalContext.hpp"
#include "ccli/exec_expr.hpp"
#include "ccli/Utility.hpp"
#include "ccli/DeclMatcher.hpp"
#include "ccli/Logger.hpp"

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


    clang::tooling::CommonOptionsParser option(argc, argv, CcliCategory, CcliUsage);
    clang::CompilerInstance CI(std::make_shared<clang::PCHContainerOperations>());

    CI.createDiagnostics();

    clang::LangOptions &lo = CI.getLangOpts();
    lo.CPlusPlus = 1;

    auto TO = std::make_shared<clang::TargetOptions>();
    TO->Triple = llvm::sys::getDefaultTargetTriple();
    clang::TargetInfo *TI = clang::TargetInfo::CreateTargetInfo(CI.getDiagnostics(), TO);
    CI.setTarget(TI);

    CI.createFileManager();
    clang::FileManager &FileManager = CI.getFileManager();
    CI.createSourceManager(FileManager);
    clang::SourceManager &SourceManager = CI.getSourceManager();

    CI.createPreprocessor(clang::TU_Module);
    CI.createASTContext();


    llvm::StringRef code = "vod foo();";
    std::string filename = "ccli.cpp";
    std::unique_ptr<llvm::MemoryBuffer> MB(llvm::MemoryBuffer::getMemBuffer(code, filename));

    clang::FrontendInputFile input_file((&MB)->get(), clang::InputKind());

    clang::FrontendOptions &FrontOpts = CI.getFrontendOpts();
    FrontOpts.Inputs = std::vector<clang::FrontendInputFile> {input_file};


    ccli::DeclFindingAction action;
    if(action.BeginSourceFile(CI, CI.getFrontendOpts().Inputs[0])) {
        action.Execute();

        auto &AstContext = CI.getASTContext();
        auto &DiagnosticsEg = AstContext.getDiagnostics();
        auto DiagnosticsCs = DiagnosticsEg.getClient();
        auto num_errs = DiagnosticsCs->getNumErrors();

        debug << "Number of errors in ASTContext: " << num_errs << std::endl;;

        action.EndSourceFile();
    }


/*
    auto &AstContext = CI.getASTContext();
    auto &DiagnosticsEg = AstContext.getDiagnostics();
    auto DiagnosticsCs = DiagnosticsEg.getClient();
    auto num_errs = DiagnosticsCs->getNumErrors();

    debug << "Number of errors in ASTContext: " << num_errs << std::endl;;


    ccli::DeclFinder MyConsumer(SourceManager);
    clang::ParseAST(CI.getPreprocessor(), &MyConsumer, CI.getASTContext());


    auto &AstContext1 = CI.getASTContext();
    auto &DiagnosticsEg1 = AstContext1.getDiagnostics();
    auto DiagnosticsCs1 = DiagnosticsEg1.getClient();
    auto num_errs1 = DiagnosticsCs1->getNumErrors();

    debug << "Number of errors in ASTContext: " << num_errs1 << std::endl;;
*/
    ccli::ClangTool Tool;
    ccli::GlobalContext global_context;
    ccli::DeclMatcher DeclMatcher;
    
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
        global_context.add_command(cmd);


        DeclMatcher.findDecl(global_context.get_context());


        Tool.run(global_context.get_context());
        std::string result = exec_expr(cmd);

        std::cout << result;
    }
    return 0;
}
