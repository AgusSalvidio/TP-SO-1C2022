include_directories(./)
set(SOURCES
       src/console_main.c
    )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(console ${SOURCES})

target_link_libraries(console utils commons)