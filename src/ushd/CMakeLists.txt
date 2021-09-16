add_subdirectory(sched)
add_subdirectory(touch)

set(EXE_USHD ushd)

set(SRC_USHD
        ushd_main.c
        # ushd_thread.c
        # ushd_sw.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/util/
    ${PROJECT_SOURCE_DIR}/src/public/
    ${PROJECT_SOURCE_DIR}/src/comm/

    ${PROJECT_SOURCE_DIR}/src/ushd/touch/
    ${PROJECT_SOURCE_DIR}/src/ushd/sched/
)


add_executable(${EXE_USHD} ${SRC_USHD})

target_link_libraries(${EXE_USHD} rt pthread ushd_touch ushd_sched ush_util)


set_target_properties(${EXE_USHD}
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
