project(USH)

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS  "-Wall \
                    -Wbool-compare \
                    -Wcast-align \
                    -Wcast-qual \
                    -Wcomments \
                    -Wconversion \
                    -Wendif-labels \
                    -Werror \
                    -Wextra \
                    -Wfloat-equal \
                    -Winline \
                    -Wlogical-op \
                    -Wmissing-prototypes \
                    -Wnested-externs \
                    -Wpointer-arith \
                    -Wsequence-point \
                    -Wshadow \
                    -Wstrict-prototypes \
                    -Wswitch-default \
                    -Wswitch-enum \
                    -Wunreachable-code \
                    -Wundef \
                    -Wwrite-strings \
                    -finline-functions \
                    -fmax-errors=20")

add_subdirectory(src)

if (USH_UT)
add_subdirectory(test)
endif()
