cmake_minimum_required(VERSION 3.4.3)

add_library(
	libs STATIC
   		src/Utils.cpp
        src/IWriters.cpp
)

target_include_directories(
	libs PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(main src/main.cpp src/Utils.cpp src/IWriters.cpp src/CompileExec.cpp)
target_link_libraries(main libs readline)

