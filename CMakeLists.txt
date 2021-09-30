project(USH)

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS "-Wall")

add_subdirectory(src)

if (USH_UT)
add_subdirectory(test)
endif()
