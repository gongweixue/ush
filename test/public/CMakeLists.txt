
set(EXE_NAME test-public)

set(SRC_LIST test-public.c)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/public/
    ${PROJECT_SOURCE_DIR}/src/comm/
    ${PROJECT_SOURCE_DIR}/src/util/
    ${PROJECT_SOURCE_DIR}/test/
)

add_executable(${EXE_NAME} ${SRC_LIST})

target_link_libraries(${EXE_NAME} rt pthread ush_util)


set_target_properties(${EXE_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_custom_command(
    TARGET ${EXE_NAME}
    POST_BUILD
    COMMAND ${TEST_OUTPUT_DIR}/${EXE_NAME} > ${TEST_LOG_FILE}
    COMMENT "Executing ${EXE_NAME} .........."
)
