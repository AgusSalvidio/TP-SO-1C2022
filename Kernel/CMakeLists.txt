include_directories(include)
set(SOURCES
        src/kernel_main.c
        src/planification.c
        src/kernel_configuration.c
        src/kernel_logs_manager.c
        src/kernel_sanitizer.c
        src/kernel_console_connection.c
        src/kernel_console_message_handler.c
        src/kernel_state_transitions.c
        src/kernel_process_image.c
        src/kernel_scheduler_queues.c
        src/kernel_memory_connection.c
        src/kernel_cpu_connection.c
        src/kernel_long_term_scheduler.c
        src/kernel_mid_term_scheduler.c
        src/kernel_short_term_scheduler.c
        src/kernel_scheduling_algorithm.c
        src/kernel_fifo_algorithm.c
        src/kernel_srt_algorithm.c
        src/kernel_event.c
        src/kernel_io_routine.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(Kernel ${SOURCES})

target_link_libraries(Kernel utils)
