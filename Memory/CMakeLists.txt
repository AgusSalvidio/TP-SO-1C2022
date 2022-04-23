include_directories(include)
set(SOURCES
        src/memory_main.c
       )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(memory ${SOURCES})

target_link_libraries(memory utils)