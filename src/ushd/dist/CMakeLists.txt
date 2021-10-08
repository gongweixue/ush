set(LIB_USHD_DIST ushd_dist)

set(SRC_USHD_DIST
    ushd_dist_thread.c
    ushd_dist_fifo.c
    ushd_dist_proc.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/util/
    ${PROJECT_SOURCE_DIR}/src/public/
    ${PROJECT_SOURCE_DIR}/src/comm/
)

add_library(${LIB_USHD_DIST} STATIC ${SRC_USHD_DIST})

target_link_libraries(${LIB_USHD_DIST}
    rt
    pthread
    ush_util
    ush_comm
)

set_target_properties(${LIB_USHD_DIST}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
