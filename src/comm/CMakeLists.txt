set(LIB_USH_COMM ush_comm)

set(SRC_USH_COMM
    lstnr/ush_comm_lstnr_hay.c
    lstnr/ush_comm_lstnr_sig_upd.c
    lstnr/ush_comm_lstnr_sigreg_ack.c
    tch/ush_comm_tch_hello.c
    realm/sig/ush_comm_realm_sigreg.c
    realm/sig/ush_comm_realm_sigset.c
)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}-Wlarger-than=128)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/comm/
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
