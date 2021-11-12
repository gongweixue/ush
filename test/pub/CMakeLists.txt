
set(EXE_NAME test-pub)

set(SRC_LIST test-pub.c)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/comm/
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/util/
    ${PROJECT_SOURCE_DIR}/test/
)

add_executable(${EXE_NAME} ${SRC_LIST})

target_link_libraries(${EXE_NAME} rt pthread ush_util)


set_target_properties(${EXE_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

if(USH_TEST_RUN)
    set(CMD ${TEST_OUTPUT_DIR}/${EXE_NAME} > ${TEST_LOG_FILE}.${EXE_NAME})
    set(COMMENT_STRING "Executing ${EXE_NAME}........")
else (USH_TEST_RUN)
    set(CMD )
    set(COMMENT_STRING "Generating ${EXE_NAME}........")
endif(USH_TEST_RUN)

add_custom_command(
    TARGET ${EXE_NAME}
    POST_BUILD
    COMMAND ${CMD}
    COMMENT ${COMMENT_STRING}
)
