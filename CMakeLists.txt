project(USH)

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS  "-I${PROJECT_SOURCE_DIR}/gen \
                    -Wall \
                    -Wbad-function-cast \
                    -Wbool-compare \
                    -Wcast-align \
                    -Wcast-qual \
                    -Wcomments \
                    -Wconversion \
                    -Wdisabled-optimization \
                    -Wdouble-promotion \
                    -Wempty-body \
                    -Wendif-labels \
                    -Werror \
                    -Wextra \
                    -Wfatal-errors \
                    -Wfloat-conversion \
                    -Wfloat-equal \
                    -Wignored-qualifiers \
                    -Winline \
                    -Wlogical-not-parentheses \
                    -Wlogical-op \
                    -Wmaybe-uninitialized \
                    -Wmemset-transposed-args \
                    -Wmissing-braces \
                    -Wmissing-declarations \
                    -Wmissing-field-initializers \
                    -Wmissing-include-dirs \
                    -Wmissing-parameter-type \
                    -Wmissing-prototypes \
                    -Wnested-externs \
                    -Woverflow \
                    -Wparentheses \
                    -Wpointer-arith \
                    -Wsign-compare \
                    -Wsizeof-pointer-memaccess \
                    -Wsequence-point \
                    -Wshadow \
                    -Wshift-count-negative \
                    -Wshift-count-overflow \
                    -Wstrict-prototypes \
                    -Wswitch-default \
                    -Wswitch-enum \
                    -Wundef \
                    -Wunreachable-code \
                    -Wunsafe-loop-optimizations \
                    -Wunused \
                    -Wunused-label \
                    -Wvla \
                    -Wwrite-strings \
                    -finline-functions \
                    -fmax-errors=10 \
                    -fno-strict-aliasing ")

add_subdirectory(src)

if (USH_UT)
set(CMAKE_BUILD_TYPE "Debug") # test cases must in debug mode anyway.
add_subdirectory(test)
endif()
