add_subdirectory(conn)
add_subdirectory(realm)
add_subdirectory(sched)
add_subdirectory(tch)
add_subdirectory(dist)

set(EXE_USHD ushd)

set(SRC_USHD
        ushd_main.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/comm/
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/ushd/
    ${PROJECT_SOURCE_DIR}/src/util/
)


add_executable(${EXE_USHD} ${SRC_USHD})

target_link_libraries(${EXE_USHD}
    ${RUNTIME_LIBS}
    ushd_tch
    ushd_sched
    ushd_dist
    ush_util
)


set_target_properties(${EXE_USHD}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
