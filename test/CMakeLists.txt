cmake_minimum_required(VERSION 3.5)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
add_executable(integration_tests integration_test.cpp)

target_link_libraries(integration_tests ${GTEST_LIBRARIES} pthread ccli)
add_test(NAME exe_tests
         COMMAND integration_tests
         DEPENDS main_test)
