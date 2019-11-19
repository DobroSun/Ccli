cmake_minimum_required(VERSION 3.5)

set(WRITER_PATH src/writers)
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
