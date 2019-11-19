cmake_minimum_required (VERSION 3.5)

set(WRITER_PATH writers)

add_library(
	libs STATIC
        exec_expr.cpp
        helpers.cpp
        IManager.cpp
        CompileExec.cpp
        Loader.cpp
        ${WRITER_PATH}/ExecWriter.cpp
        ${WRITER_PATH}/NormWriter.cpp
        ${WRITER_PATH}/PreWriter.cpp
        ${WRITER_PATH}/NormOpWriter.cpp
        ${WRITER_PATH}/PreOpWriter.cpp
)
