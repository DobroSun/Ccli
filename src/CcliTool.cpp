#include "clang/Tooling/Tooling.h"

#include "ccli/CcliTool.hpp"
#include "ccli/Logger.hpp"


namespace ccli {
// CcliTool runs without args.
//std::vector<std::string> args;
/*
bool CcliTool::run(std::string code) {
    return clang::tooling::runToolOnCodeWithArgs(
                std::unique_ptr<clang::ento::AnalysisAction>(),
                code,
                args,
                "ccli.cpp",
                "main",
                std::make_shared<clang::PCHContainerOperations>(),
                std::vector<std::pair<std::string, std::string>>());

}
*/
CcliTool::CcliTool(clang::CompilerInstance &CI_) {
    CI = &CI_;
    std::string filename = "ccli.cpp";
}
/*
void CcliTool::init_CI() {
    CI->createDiagnostics();

    auto TO = std::make_shared<clang::TargetOptions>();
    TO->Triple = llvm::sys::getDefaultTargetTriple();
    clang::TargetInfo *TI = clang::TargetInfo::CreateTargetInfo(CI->getDiagnostics(), TO);
    CI->setTarget(TI);

    CI->createFileManager();
    clang::FileManager &FileManager = CI->getFileManager();
    CI->createSourceManager(FileManager);
    clang::SourceManager &SourceManager = CI->getSourceManager();

    CI->createPreprocessor(clang::TU_Module);
    CI->createASTContext();
}
*/

bool CcliTool::run(clang::FrontendAction *ToolAction) {
    std::string cmd = "int a;";
    llvm::StringRef code = cmd;

    std::unique_ptr<llvm::MemoryBuffer> MB(llvm::MemoryBuffer::getMemBuffer(code, filename));

    // Create VirtualFile
    clang::FrontendInputFile input_file((&MB)->get(), clang::InputKind());
    debug << MB.get()->getBuffer() << std::endl;

    clang::FrontendOptions &FrontOpts = CI->getFrontendOpts();
    FrontOpts.Inputs = std::vector<clang::FrontendInputFile> {input_file};

    debug << FrontOpts.Inputs[0] << " <- This is FrontendOPts" << std::endl;
    //if(ToolAction->BeginSourceFile(*CI, CI->getFrontendOpts().Inputs[0])) {
/*
        ToolAction->Execute();

        auto &AstContext = CI->getASTContext();
        auto &DiagnosticsEg = AstContext.getDiagnostics();
        auto DiagnosticsCs = DiagnosticsEg.getClient();
        int num_errs = DiagnosticsCs->getNumErrors();
        debug << "Number of errors in ASTContext: " << num_errs << std::endl;;

        ToolAction->EndSourceFile();
*/
    //}

    return true;
}

int CcliTool::get_errs() const {
    return num_errs;
}


} // namespace
