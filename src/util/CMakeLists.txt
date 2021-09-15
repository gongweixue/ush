set(LIB_UTIL util)

set(SRC_UTIL
    ush_log.c
)

include_directories(${PROJECT_SOURCE_DIR}/src/util/)

add_library(${LIB_UTIL} STATIC ${SRC_UTIL})

target_link_libraries(${LIB_UTIL} pthread)

set_target_properties(${LIB_UTIL}
                      PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
