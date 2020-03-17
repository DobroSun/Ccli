cmake_minimum_required(VERSION 3.5)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
add_executable(run_tests
    test.cpp
    utility/test.cpp
)

message(STATUS "Gtest include directory: ${GTEST_INCLUDE_DIR}")
message(STATUS "Gtest headers: ${GTEST_LIBRARIES}")

target_link_libraries(run_tests
    ${GTEST_LIBRARIES}
    pthread
    ccli
)

add_test(NAME Test
         COMMAND run_tests
)
