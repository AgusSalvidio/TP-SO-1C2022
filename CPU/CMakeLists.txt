include_directories(include)
set(SOURCES
        src/cpu_main.c
        src/cpu_configuration_manager.c
        src/cpu_logs_manager.c
        src/cpu_sanitizer.c
        src/cpu_memory_connection_handler.c
        src/cpu_kernel_dispatch_connection_handler.c
        src/cpu_manager.c
        src/cpu_serializable_objects.c
        src/cpu_query_performer.c
        src/cpu_kernel_interrupt_connection_handler.c
        src/cpu_instruction_cycle.c
        src/cpu_action_performer.c
        src/cpu_mmu.c src/cpu_tlb.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(cpu ${SOURCES})

target_link_libraries(cpu utils)