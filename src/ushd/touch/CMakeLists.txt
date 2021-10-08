set(LIB_USHD_TOUCH ushd_touch)

set(SRC_USHD_TOUCH
    ushd_touch.c
    ushd_touch_thread.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/comm/
    ${PROJECT_SOURCE_DIR}/src/util/
    ${PROJECT_SOURCE_DIR}/src/pub/
    ${PROJECT_SOURCE_DIR}/src/ushd/sched/
)

add_library(${LIB_USHD_TOUCH} STATIC ${SRC_USHD_TOUCH})

target_link_libraries(${LIB_USHD_TOUCH}
    rt
    pthread
    ush_util
)

set_target_properties(${LIB_USHD_TOUCH}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
