cmake_minimum_required (VERSION 3.5)

add_library(
	ccli STATIC
        exec_expr.cpp
)

#target_include_directories(ccli PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
target_include_directories(ccli PRIVATE "${LLVM_INCLUDE_DIR}")

target_link_libraries(
    ccli PRIVATE
        clangTooling
        clangFrontend
)
