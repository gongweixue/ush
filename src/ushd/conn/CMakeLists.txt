set(LIB_USHD_CONN ushd_conn)

set(SRC_USHD_CONN
    ushd_conn_record_tbl.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/comm/
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/ushd/
    ${PROJECT_SOURCE_DIR}/src/util/
)

add_library(${LIB_USHD_CONN} STATIC ${SRC_USHD_CONN})

target_link_libraries(${LIB_USHD_CONN}
    rt
    pthread
    ush_util
    ush_comm
    ushd_reg
)

set_target_properties(${LIB_USHD_CONN}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
