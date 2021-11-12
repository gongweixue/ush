set(LIB_USHD_REALM ushd_realm)

set(SRC_USHD_REALM
    ushd_realm_thread.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/comm/
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/ushd/
    ${PROJECT_SOURCE_DIR}/src/util/
)

add_library(${LIB_USHD_REALM} STATIC ${SRC_USHD_REALM})

target_link_libraries(${LIB_USHD_REALM}
    ${RUNTIME_LIBS}
    ush_comm
    ush_util
)

set_target_properties(${LIB_USHD_REALM}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
