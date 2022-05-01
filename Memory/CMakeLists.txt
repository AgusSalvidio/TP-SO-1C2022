include_directories(include)
set(SOURCES
        src/memory_main.c
        src/memory_configuration_manager.c
        src/memory_logs_manager.c
        src/memory_serializable_objects.c
        src/memory_query_performers.c
        src/memory_manager.c
        src/memory_connection_handler.c
        src/memory_sanitizer.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(memory ${SOURCES})

target_link_libraries(memory utils)