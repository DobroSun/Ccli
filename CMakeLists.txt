cmake_minimum_required(VERSION 3.5)

set(WRITER_PATH src/writers)

# Fixes clang linking error.
add_definitions(-DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING)
add_subdirectory(src)

target_include_directories(
	libs PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(
    main 
        src/main.cpp
)

target_link_libraries(main libs readline)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()
