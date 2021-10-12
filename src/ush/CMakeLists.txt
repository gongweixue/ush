set(LIB_USH ush)

set(SRC_USH
    ush_connect.c
    ush_sync.c
    ush_touch.c
    ush_version.c
    listener/ush_listener.c
    listener/proc/ush_listener_proc.c
    listener/proc/ush_listener_proc_howareyou.c
    pipe/ush_pipe_create.c
    pipe/ush_pipe_attr.c
    sig/ush_sig_reg.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/comm/
    ${PROJECT_SOURCE_DIR}/src/plugin/
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/ush/
    ${PROJECT_SOURCE_DIR}/src/util/
)

add_library(${LIB_USH} STATIC ${SRC_USH})

target_link_libraries(${LIB_USH}  rt pthread ush_util ush_comm)

set_target_properties(${LIB_USH}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
