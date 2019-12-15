#include "clang/Tooling/Tooling.h"

#include "ccli/Logger.hpp"
#include "ccli/CcliTool.hpp"


namespace ccli {

CcliTool::CcliTool(clang::CompilerInstance &CI_, clang::HeaderSearch &HS_) {
    CI = &CI_;
    HS = &HS_;
    std::string filename = "ccli.cpp";
}


bool CcliTool::run(clang::FrontendAction *ToolAction, std::string &cmd) {
    // Create MemoryBuffer and InputFile.
    llvm::StringRef code = cmd;
    std::unique_ptr<llvm::MemoryBuffer> MB = llvm::MemoryBuffer::getMemBufferCopy(code);
    clang::FrontendInputFile InputFile((&MB)->get(), clang::InputKind());


    // Just debug.
    llvm::MemoryBuffer *buffer = InputFile.getBuffer();
    std::string str(buffer->getBufferStart(), buffer->getBufferEnd());
    debug() << "String of File content: " << str <<  std::endl;

    // Is this necessary to define?
    clang::FrontendOptions &FrontOpts = CI->getFrontendOpts();
    FrontOpts.Inputs = std::vector<clang::FrontendInputFile> {InputFile};


    clang::DiagnosticsEngine &DiagnosticsEngine = CI->getDiagnostics();
    clang::FileManager &FileManager = CI->getFileManager();
    clang::SourceManager &SourceManager = CI->getSourceManager();
    clang::DependencyOutputOptions DepOutputOpts = CI->getDependencyOutputOpts();

    CI->InitializeSourceManager(
                InputFile,
                DiagnosticsEngine,
                FileManager,
                SourceManager,
                HS,
                DepOutputOpts,
                FrontOpts);


    if(ToolAction->BeginSourceFile(*CI, InputFile)) {
        bool res = ToolAction->Execute();
        if(!res) {
            debug() << "Execution of action failed!" << std::endl;
            return false;
        }
        
        // Get number of errors occured.
        clang::ASTContext &AstContext = CI->getASTContext();
        clang::DiagnosticsEngine &DiagnosticsEg = AstContext.getDiagnostics();
        clang::DiagnosticConsumer *DiagnosticsCs = DiagnosticsEg.getClient();
        num_errs = DiagnosticsCs->getNumErrors();

        debug() << num_errs << " <- Number of errors in ASTContext" << std::endl;
        ToolAction->EndSourceFile();
    }
    return (!get_errs())? true: false;
}
/*
bool CcliTool::Init_SourceManager() {

}
*/
int CcliTool::get_errs() const {
    return num_errs;
}

} // namespace
