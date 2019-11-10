cmake_minimum_required(VERSION 3.4.3)

set(WRITER_PATH src/writers)

add_library(
	libs STATIC
        src/IManager.cpp
        src/CompileExec.cpp
        ${WRITER_PATH}/ExecWriter.cpp
        ${WRITER_PATH}/NormWriter.cpp
        ${WRITER_PATH}/PreWriter.cpp
        ${WRITER_PATH}/NormOpWriter.cpp
        ${WRITER_PATH}/PreOpWriter.cpp
        src/helpers.cpp
)

target_include_directories(
	libs PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(main src/main.cpp src/IManager.cpp src/CompileExec.cpp src/helpers.cpp ${WRITER_PATH}/ExecWriter.cpp ${WRITER_PATH}/NormWriter.cpp ${WRITER_PATH}/NormOpWriter ${WRITER_PATH}/PreWriter ${WRITER_PATH}/PreOpWriter.cpp)
target_link_libraries(main libs readline)

