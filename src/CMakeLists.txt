cmake_minimum_required (VERSION 3.5)

set(WRITER_PATH writers)

add_library(
	libs STATIC
        exec_expr.cpp
        #helpers.cpp
)
