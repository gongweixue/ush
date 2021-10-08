set(LIB_USH_SIGNAL ush_signal)

set(SRC_USH_SIGNAL
    ush_signal_access.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src/plugin/
)

add_library(${LIB_USH_SIGNAL} STATIC ${SRC_USH_SIGNAL})

target_link_libraries(${LIB_USH_SIGNAL}
    rt
    pthread
)

set_target_properties(${LIB_USH_SIGNAL}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)
