include_directories(include)
set(SOURCES
        src/cpu_main.c
       )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(cpu ${SOURCES})

target_link_libraries(cpu utils)