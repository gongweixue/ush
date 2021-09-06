set(LIB_USH ush)

set(SRC_USH    ush_pipe.c
               ush_version.c)

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/src/common)

add_library(${LIB_USH} STATIC ${SRC_USH})

# target_link_libraries(${LIB_USH} asound)

set_target_properties(${LIB_USH}
                      PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
