set(LIB_USH ush)

set(SRC_USH
    ush_connect.c
    ush_sync.c
    ush_tch.c
    ush_version.c
    lstnr/ush_lstnr.c
    lstnr/proc/ush_lstnr_proc.c
    lstnr/proc/ush_lstnr_proc_hay.c
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
