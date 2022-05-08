include_directories(include)
set(SOURCES
        src/kernel_main.c
        include/structs.h include/planification.h src/planification.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(kernel ${SOURCES})

target_link_libraries(kernel utils)