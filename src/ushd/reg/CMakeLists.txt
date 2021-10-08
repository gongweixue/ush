set(LIB_USHD_REG ushd_reg)

set(SRC_USHD_REG
    ushd_reg_list.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/public/
    ${PROJECT_SOURCE_DIR}/src/util/
    ${PROJECT_SOURCE_DIR}/src/comm/
)

add_library(${LIB_USHD_REG} STATIC ${SRC_USHD_REG})

target_link_libraries(${LIB_USHD_REG}
    ush_util
)

set_target_properties(${LIB_USHD_REG}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
