#!/bin/bash
set -e
rm -f arm-rtos.bin arm-rtos.elf
make
arm-none-eabi-objcopy -O binary arm-rtos.elf arm-rtos.bin
