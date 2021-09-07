set(EXE_USH_SRV ush_srv)

set(SRC_USH_SRV    ush_srv_main.c
                   ush_srv_thread.c
                   ush_srv_touch.c
                   ush_srv_sw.c)

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/src/common)


add_executable(${EXE_USH_SRV} ${SRC_USH_SRV})

target_link_libraries(${EXE_USH_SRV} rt pthread)


set_target_properties(${EXE_USH_SRV}
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
