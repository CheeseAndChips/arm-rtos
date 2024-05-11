#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "hal.h"

const char *fmt = "Reset vector: %p\n";

void c_entry(void) {
    clock_init();
    enable_rccs();
    set_gpio_mode(GPIOB, 7, 1);
    uart_init();
    char buffer[128];
    char *ptr = (char*)(*(uint32_t*)(0x8000000));
    uart_send("Starting snprintf\n");
    snprintf(buffer, sizeof(buffer), fmt, ptr);
    uart_send(buffer);
    for(;;) {
        for(int i = 0; i < 1000000; i++);
        set_gpio_output(GPIOB, 7, 1);
        for(int i = 0; i < 1000000; i++);
        set_gpio_output(GPIOB, 7, 0);
    }
}
