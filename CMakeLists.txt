cmake_minimum_required(VERSION 3.13)

find_program(STRIPIOS stripios REQUIRED)

set(BOULEVARD_MAJOR 0)
set(BOULEVARD_MINOR 0)
set(BOULEVARD_PATCH 1)
execute_process(COMMAND
    git describe --dirty --always --exclude "*"
    OUTPUT_VARIABLE BOULEVARD_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

project(
    boulevard
    VERSION ${BOULEVARD_MAJOR}.${BOULEVARD_MINOR}.${BOULEVARD_PATCH}
    LANGUAGES ASM C
)

set(ARCH -mcpu=arm926ej-s -mthumb -mthumb-interwork -mbig-endian)

set(CMAKE_C_STANDARD 11)

add_compile_options(
    ${ARCH}
    -O2
    -g3
    -ffreestanding
    -fomit-frame-pointer
    -ffunction-sections
    -Wall
    -fno-builtin
)

add_executable(boulevard
    source/start.s
    source/heap.c
    source/main.c
    source/network.c
    source/libc.c
)

target_include_directories(boulevard PRIVATE
    include
    cios-lib
)

target_compile_definitions(boulevard PRIVATE
    BOULEVARD_MAJOR=${BOULEVARD_MAJOR}
    BOULEVARD_MINOR=${BOULEVARD_MINOR}
    BOULEVARD_PATCH=${BOULEVARD_PATCH}
    BOULEVARD_HASH=${BOULEVARD_HASH}
)

target_link_libraries(boulevard PRIVATE
    cios-lib
    gcc
)

target_link_options(boulevard PRIVATE
    ${ARCH}
    -nostartfiles
    -nostdlib
    -Wl,-T,${CMAKE_CURRENT_SOURCE_DIR}/link.ld,-Map,${CMAKE_PROJECT_NAME}.map
    -Wl,--gc-sections
    -Wl,-static
)

add_custom_target(app ALL
    DEPENDS ${CMAKE_BINARY_DIR}/BOULEVARD.app
)

add_custom_command(
    DEPENDS ${CMAKE_PROJECT_NAME}
    OUTPUT ${CMAKE_BINARY_DIR}/BOULEVARD.app
    COMMAND ${STRIPIOS} ${CMAKE_PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/BOULEVARD.app
)

add_subdirectory(cios-lib)
