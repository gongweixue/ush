set(LIB_USHD_PUBLISH ushd_publish)

set(SRC_USHD_PUBLISH
    ushd_publish_thread.c
    ushd_publish_fifo.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/util/
    ${PROJECT_SOURCE_DIR}/src/public/
)

add_library(${LIB_USHD_PUBLISH} STATIC ${SRC_USHD_PUBLISH})

target_link_libraries(${LIB_USHD_PUBLISH}
    rt
    pthread
    ush_util
    ush_comm
)

set_target_properties(${LIB_USHD_PUBLISH}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
