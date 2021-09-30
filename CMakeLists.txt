project(USH)

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS "-Wall")

if (USH_TEST)
    add_definitions(-DUSH_LOG_ON=0)
endif()

add_subdirectory(src)
add_subdirectory(test)
