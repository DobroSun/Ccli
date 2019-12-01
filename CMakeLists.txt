cmake_minimum_required(VERSION 3.5)

project(ccli
        VERSION 0.0.1
        DESCRIPTION "A toy implementaion of C++ interpreter"
        LANGUAGES CXX)

# Fixes clang linking error.
add_definitions(-DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} in ${LLVM_INSTALL_PREFIX}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

find_package(Clang REQUIRED CONFIG HINTS "{LLVM_INSTALL_PREFIX}/lib/cmake/clang")
message(STATUS "Found Clang in ${CLANG_INSTALL_PREFIX}")

add_subdirectory(src)

target_include_directories(
	ccli PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(
    main 
        src/main.cpp
)

target_link_libraries(main ccli readline)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
