cmake_minimum_required(VERSION 3.20)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(arm-rtos C ASM)

set(C_SOURCES
    ./src/main.c
)

set(ASM_SOURCES
    ./src/startup.s
)

set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -nostartfiles")
add_executable(${PROJECT_NAME} ${C_SOURCES} ${ASM_SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -mcpu=arm926ej-s -g)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
    DEPENDS ${PROJECT_NAME}
    VERBATIM
)
