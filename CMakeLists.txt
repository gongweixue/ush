project(USH)

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS "-Wall \
                   -Werror \
                   -Wextra \
                   -Wsequence-point \
                   -Wswitch-default \
                   -Wswitch-enum \
                   -Wbool-compare \
                   -Wtautological-compare \
                   -Wfloat-equal \
                   -Wpointer-arith \
                   -Wcast-align \
                   -Wcast-qual \
                   -Wwrite-strings \
                   -Wlogical-op \
                   -Wconversion \
                   -Wshadow \
                   -Winline \
                   -Wunreachable-code \
                   -Wundef \
                   -Wstrict-prototypes \
                   -finline-functions \
                   -Wmissing-prototypes \
                   -Wnested-externs")

add_subdirectory(src)

if (USH_UT)
add_subdirectory(test)
endif()
