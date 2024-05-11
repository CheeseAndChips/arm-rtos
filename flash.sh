#!/bin/bash
set -e
./build.sh
st-flash --reset write arm-rtos.bin 0x8000000
