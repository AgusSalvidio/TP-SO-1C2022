include_directories(include)

set(SOURCES src/config_updater.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(config_updater ${SOURCES})

target_link_libraries(config_updater utils -lrt -lreadline)
