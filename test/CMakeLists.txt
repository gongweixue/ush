
set(EXE_TEST_PIPE_CREATE pipe-create-test)

set(SRC_TEST_PIPE_CREATE    test_pipe_create.c)

include_directories(${PROJECT_SOURCE_DIR}/src/ush/
                    ${PROJECT_SOURCE_DIR}/src/public/)

add_executable(${EXE_TEST_PIPE_CREATE} ${SRC_TEST_PIPE_CREATE})

target_link_libraries(${EXE_TEST_PIPE_CREATE} ush rt c)


set_target_properties(${EXE_TEST_PIPE_CREATE}
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
