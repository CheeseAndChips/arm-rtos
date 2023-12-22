#!/bin/bash

set -e

mkdir -p ./build
cd ./build
cmake --toolchain ../toolchain.cmake ../ && make
qemu-system-arm -M stm32vldiscovery -m 128M -nographic -kernel ./arm-rtos.bin
