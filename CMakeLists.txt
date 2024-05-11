cmake_minimum_required(VERSION 3.20)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(arm-rtos.elf C ASM)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)

set(C_SOURCES
    ./src/main.c
    ./src/hal.c
    ./src/syscalls.c
)

set(ASM_SOURCES
    ./src/startup.s
)

set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -mcpu=cortex-m4 --specs=nosys.specs -Wl,--gc-sections -Wl,-Map=arm-rtos.map --specs=nano.specs -mthumb")
add_executable(${PROJECT_NAME} ${C_SOURCES} ${ASM_SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -mcpu=cortex-m4 -mthumb -g)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
    DEPENDS ${PROJECT_NAME}
    VERBATIM
)
