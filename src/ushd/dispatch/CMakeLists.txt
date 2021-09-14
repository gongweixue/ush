set(LIB_USHD_DISPATCH ushd_dispatch)

set(SRC_USHD_DISPATCH
    ushd_dispatch.c
)

include_directories(${PROJECT_SOURCE_DIR}/src/comm/
                    ${PROJECT_SOURCE_DIR}/src/util/
                    ${PROJECT_SOURCE_DIR}/src/public/
)

add_library(${LIB_USHD_DISPATCH} STATIC ${SRC_USHD_DISPATCH})

target_link_libraries(${LIB_USHD_DISPATCH} pthread)

set_target_properties(${LIB_USHD_DISPATCH}
                      PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
