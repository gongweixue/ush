set(LIB_COMM comm)

set(SRC_COMM
    ush_comm_hello_msg.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/util/
)

add_library(${LIB_COMM} STATIC ${SRC_COMM})

target_link_libraries(${LIB_COMM} pthread util)

set_target_properties(${LIB_COMM}
                      PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
