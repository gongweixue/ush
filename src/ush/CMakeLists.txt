set(LIB_USH ush)

set(SRC_USH
    ush_connect.c
    ush_cr.c
    ush_hello.c
    ush_listener.c
    ush_pipe_create.c
    ush_sync.c
    ush_touch.c
    ush_version.c
)

include_directories(${PROJECT_SOURCE_DIR}/src/ush/
                    ${PROJECT_SOURCE_DIR}/src/util/
                    ${PROJECT_SOURCE_DIR}/src/public/
                    ${PROJECT_SOURCE_DIR}/src/comm/)

add_library(${LIB_USH} STATIC ${SRC_USH})

target_link_libraries(${LIB_USH} pthread util)

set_target_properties(${LIB_USH}
                      PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
