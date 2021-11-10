
set(EXE_NAME test-ush-all)

set(SRC_LIST
    main.c
    case_ush_all_thread_0.c
    # case_thread_1.c
    # case_thread_2.c
    # case_thread_3.c
    # case_thread_4.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/util/
    ${PROJECT_SOURCE_DIR}/test/
)

add_executable(${EXE_NAME} ${SRC_LIST})

target_link_libraries(${EXE_NAME} ush ush_comm)


set_target_properties(${EXE_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

# add_custom_command(
#     TARGET ${EXE_NAME}
#     POST_BUILD
#     COMMAND ${TEST_OUTPUT_DIR}/${EXE_NAME}
#     COMMENT "Executing ${EXE_NAME} .........."
# )
