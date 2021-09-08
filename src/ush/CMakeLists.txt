set(LIB_USH ush)

set(SRC_USH    ush_pipe_create.c
               ush_version.c
               ush_pipe_cr.c)

include_directories(${PROJECT_SOURCE_DIR}/src/ush/
                    ${PROJECT_SOURCE_DIR}/src/log/
                    ${PROJECT_SOURCE_DIR}/src/public/
                    ${PROJECT_SOURCE_DIR}/src/comm/)

add_library(${LIB_USH} STATIC ${SRC_USH})

target_link_libraries(${LIB_USH} pthread)

set_target_properties(${LIB_USH}
                      PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
