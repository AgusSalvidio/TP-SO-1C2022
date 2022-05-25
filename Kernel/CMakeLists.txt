include_directories(include)
set(SOURCES
        src/kernel_main.c
        src/kernel_configuration.c
        src/kernel_logs_manager.c
        src/kernel_sanitizer.c
        src/kernel_console_connection.c
        src/kernel_console_message_handler.c
        src/kernel_state_transitions.c
        src/kernel_process_image.c
        src/kernel_scheduler_queues.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(Kernel ${SOURCES})

target_link_libraries(Kernel utils)