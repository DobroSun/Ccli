cmake_minimum_required (VERSION 3.5)

#set(LLVM_LINK_COMPONENTS
#    ${LLVM_TARGETS_TO_BUILD}
#    Option
#    Support
#)

add_library(
	ccli STATIC
        exec_expr.cpp
        runToolOnCode.cpp
        GlobalContext.cpp
        Utility.cpp
)

target_include_directories(ccli PRIVATE "${LLVM_INCLUDE_DIR}")

target_link_libraries(
    ccli PRIVATE
        clangTooling
        clangFrontend
        clangStaticAnalyzerFrontend
)
