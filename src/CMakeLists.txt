cmake_minimum_required (VERSION 3.5)

add_library(
	ccli STATIC
		utility.cpp
    context.cpp
)

target_include_directories(
	ccli PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
