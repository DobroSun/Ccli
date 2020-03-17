cmake_minimum_required(VERSION 3.5)

project(main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} in ${LLVM_INSTALL_PREFIX}")
message(STATUS "Using LLVMConfig.cmake in ${LLVM_DIR}")

find_package(Clang REQUIRED CONFIG HINTS "{LLVM_INSTALL_PREFIX}/lib/cmake/clang")
message(STATUS "Found Clang in ${CLANG_INSTALL_PREFIX}")
message(STATUS "Found Cland headers in ${CLANG_INCLUDE_DIRS}")
message(STATUS "Found LLVM headers in ${LLVM_INCLUDE_DIRS}")

add_subdirectory(src)

add_executable(
    main 
        src/main.cpp
)


option(DEFINE_DEBUG "Build the project using debugging code" OFF)
if(DEFINE_DEBUG)
    target_compile_definitions(main PUBLIC -DDEBUG)
    set(CMAKE_BUILD_TYPE Debug)
endif(DEFINE_DEBUG)


# Fixes clang linking error.
target_compile_definitions(main PUBLIC -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING)
target_link_libraries(main ccli readline)

message(STATUS "Build type is " ${CMAKE_BUILD_TYPE})

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
