include_directories(include)
set(SOURCES
        src/console_main.c
        src/console_logs_manager.c
        src/console_sanitizer.c
        )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(Console ${SOURCES})

target_link_libraries(Console utils)