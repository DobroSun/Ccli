cmake_minimum_required (VERSION 3.5)

set(WRITER_PATH writers)

include_directories(/usr/lib/llvm-6.0/include)
add_library(
	libs STATIC
        exec_expr.cpp
)
