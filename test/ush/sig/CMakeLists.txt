
set(EXE_NAME test-ush-sig)

set(SRC_LIST test-ush-sig.c)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/plugin/
    ${PROJECT_SOURCE_DIR}/src/ushd/sched/
    ${PROJECT_SOURCE_DIR}/src/ushd/touch/
    ${PROJECT_SOURCE_DIR}/src/util/
    ${PROJECT_SOURCE_DIR}/test/
)

add_executable(${EXE_NAME} ${SRC_LIST})

target_link_libraries(${EXE_NAME} ush ush_comm ushd_sched ushd_touch)


set_target_properties(${EXE_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

add_custom_command(
    TARGET ${EXE_NAME}
    POST_BUILD
    COMMAND ${TEST_OUTPUT_DIR}/${EXE_NAME} > ${TEST_LOG_FILE}
    COMMENT "Executing ${EXE_NAME} .........."
)
