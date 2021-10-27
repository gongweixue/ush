set(LIB_USHD_SCHED ushd_sched)

set(SRC_USHD_SCHED
    ushd_sched_thread.c
    ushd_sched_fifo.c
    proc/ushd_sched_proc.c
    proc/tch/ushd_sched_proc_tch.c
    proc/tch/ushd_sched_proc_tch_hello.c
    proc/realm/ushd_sched_proc_realm.c
    proc/realm/ushd_sched_proc_realm_sig.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/comm/
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/ushd/
    ${PROJECT_SOURCE_DIR}/src/util/
)

add_library(${LIB_USHD_SCHED} STATIC ${SRC_USHD_SCHED})

target_link_libraries(${LIB_USHD_SCHED}
    rt
    ush_comm
    ush_util
    ushd_conn
    ushd_dist
    ushd_realm
)

set_target_properties(${LIB_USHD_SCHED}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
