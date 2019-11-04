cmake_minimum_required(VERSION 3.4.3)

add_library(
	libs STATIC
   		src/Utils.cpp
)

target_include_directories(
	libs PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(
	libs PRIVATE
    -std=c++11
)

add_executable(main src/main.cpp src/Utils.cpp)
target_link_libraries(main libs)
