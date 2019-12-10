cmake_minimum_required (VERSION 3.5)

add_library(
	ccli STATIC
        exec_expr.cpp
        runToolOnCode.cpp
        GlobalContext.cpp
        Utility.cpp
        DeclMatcher.cpp
)

target_include_directories(ccli PUBLIC "${LLVM_INCLUDE_DIR}")
target_include_directories(
	ccli PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
    ccli PRIVATE
        clangTooling
        clangFrontend
        clangStaticAnalyzerFrontend
        clangAST
)
