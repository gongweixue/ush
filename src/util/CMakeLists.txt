set(LIB_UTIL ush_util)

set(SRC_UTIL
    ush_log.c
)

include_directories(${PROJECT_SOURCE_DIR}/src/util/)

add_library(${LIB_UTIL} STATIC ${SRC_UTIL})


set_target_properties(${LIB_UTIL}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
