cmake_minimum_required(VERSION 3.1...3.29)

set(PROJECT_NAME emulator_hub)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    ${PROJECT_NAME}
    VERSION 1.0
    LANGUAGES C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/rom DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} src/entry.c
                               src/mem/mem.c 
                               src/dev/cpu/sharp_sm83/sharp_sm83_cpu.c
                               src/dev_manager/gameboy_dmg_01/gameboy_dmg_01.c
                               src/interface/interface_manager.c)

target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::GTK4 PkgConfig::SDL2)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
