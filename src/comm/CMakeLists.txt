set(LIB_USH_COMM ush_comm)

set(SRC_USH_COMM
    ush_comm_hello_msg.c
    ush_comm_howareyou_msg.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/util/
)

add_library(${LIB_USH_COMM} STATIC ${SRC_USH_COMM})

target_link_libraries(${LIB_USH_COMM}
    rt
    ush_util
)

set_target_properties(${LIB_USH_COMM}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)
