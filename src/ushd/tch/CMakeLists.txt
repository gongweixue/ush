set(LIB_USHD_TCH ushd_tch)

set(SRC_USHD_TCH
    ushd_tch.c
    ushd_tch_thread.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/comm/
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/ushd/
    ${PROJECT_SOURCE_DIR}/src/util/
)

add_library(${LIB_USHD_TCH} STATIC ${SRC_USHD_TCH})

target_link_libraries(${LIB_USHD_TCH}
    ${RUNTIME_LIBS}
    ush_util
)

set_target_properties(${LIB_USHD_TCH}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
