cmake_minimum_required(VERSION 3.1...3.29)

set(PROJECT_NAME emulator_hub)

project(
    ${PROJECT_NAME}
    VERSION 1.0
    LANGUAGES C)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/rom DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} src/entry.c
                               src/mem/mem.c 
                               src/dev/cpu/sharp_sm83/sharp_sm83_cpu.c
                               src/dev_manager/gameboy_dmg_01/gameboy_dmg_01.c)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
