#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Lex/PreprocessorOptions.h"


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


/*
static int Execute(std::unique_ptr<llvm::Module> Mod, char *const *envp) {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();

  std::string Error;
  llvm::ExecutionEngine *EE = llvm::EngineBuilder(std::move(Mod)).setErrorStr(&Error).create();


  if (!EE) {
    std::cerr << "unable to make execution engine!" << std::endl;
    return 255;
  }

  llvm::Function *EntryFn = (*Mod).getFunction("main");
  if (!EntryFn) {
    std::cerr << "'main' function not found in module.\n";
    return 255;
  }

  // FIXME: Support passing arguments.
  std::vector<std::string> Args;
  Args.push_back((*Mod).getModuleIdentifier());

  EE->finalizeObject();
  return EE->runFunctionAsMain(EntryFn, Args, envp);
}
*/


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
    std::string r = "-triple=" + llvm::sys::getDefaultTargetTriple();
    headers.push_back(r.c_str());
    print(headers);


    llvm::ArrayRef<const char*> args(headers);
    // Initialize Invocation
    std::shared_ptr<clang::CompilerInvocation> CInv(new clang::CompilerInvocation);
    CInv->CreateFromArgs(*(CInv.get()), args.begin(), args.end(), DiagnosticsEngine);
    CI.setInvocation(CInv);


    clang::FrontendOptions &FrontOpts = CI.getFrontendOpts();
    FrontOpts.Inputs.clear();
    debug() << "******** CompilerInstance is initialized ********" << std::endl;
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
    //clang::tooling::CommonOptionsParser option(argc, argv, CcliCategory, CcliUsage);

    clang::CompilerInstance CI(std::make_shared<clang::PCHContainerOperations>());
    Init_CI(CI);

    clang::tooling::CommonOptionsParser ToolOptions(argc, argv, CcliCategory);
    ccli::CcliTool Tool(ToolOptions.getCompilations(),
                        ToolOptions.getSourcePathList());





    ccli::GlobalContext GlobalContext;
    ccli::StateScope Scope;
    ccli::StateManager StateManager(GlobalContext);

    std::unique_ptr<clang::tooling::FrontendActionFactory> DumpFact =
        clang::tooling::newFrontendActionFactory<ccli::DumpASTAction>();



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
        // Frontend Actions that will be processed
        // With CcliTool.
        ccli::DeclFindingAction *FindAct = new ccli::DeclFindingAction;
        ccli::DumpASTAction *DumpAct = new ccli::DumpASTAction;
        clang::ento::AnalysisAction *AnalysisAct = new clang::ento::AnalysisAction;
        clang::SyntaxOnlyAction *SyntaxOnlyActnew = new clang::SyntaxOnlyAction;

        const char *cmd = readline(welcome().c_str());

        // Handles C-d and whitespace
        if(cmd == NULL) {std::cout << "\n"; exit(0);}
        if(!std::strcmp(cmd, "")) continue;
        add_history(cmd);


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

        debug() << "Before DUMPACT.get()" << std::endl;
#ifdef DEBUG
        Tool.execute(DumpAct, context_string);
#endif



/*
        std::unique_ptr<llvm::MemoryBuffer> MB = llvm::MemoryBuffer::getMemBufferCopy(context_string);
        
        CI.getPreprocessorOpts().addRemappedFile("ccli.cpp", MB.get());

        llvm::LLVMContext LLVMContext;
        std::unique_ptr<clang::CodeGenAction> LLVMAct(new clang::EmitLLVMOnlyAction(&LLVMContext));
        if(!CI.ExecuteAction(*LLVMAct)) {
            std::cerr << "CodeGeneration failed!" << std::endl;
            exit(0);
        }
*/
/*
        std::unique_ptr<llvm::Module> Module = LLVMAct->takeModule();
        if(!Module) {
            std::cerr << "Module not exists!" << std::endl;
            exit(0);
        }


        char* const*envp;
        int errs_code = Execute(std::move(Module), envp);
        if(errs_code) {
            std::cerr << "Execution of module failed!" << std::endl;
            exit(0);
        }
*/

        llvm::LLVMContext LLVMContext;
        std::unique_ptr<llvm::MemoryBuffer> MB = llvm::MemoryBuffer::getMemBufferCopy(context_string);
        //std::unique_ptr<clang::EmitLLVMOnlyAction> LLVMAct(new clang::EmitLLVMOnlyAction(&LLVMContext));

        //clang::EmitLLVMOnlyAction LLVMAct(&LLVMContext);
        //clang::FrontendInputFile InputFile(MB.get(), clang::InputKind());
        //bool res = clang::tooling::runToolOnCode(LLVMAct.get(), "int a;");

        std::string result = exec_expr(cmd);
        std::cout << result;
    }
    return 0;
}
