#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "hal.h"

const char *fmt_init = "Initial stack pointer: %p\n";
const char *fmt_other = "Other function stack pointer: %p\n";
const char other_stack[16 * 1024];
const char *other_stack_end;

// void _pendsv_handler(void) {
//     uart_send("PendSV\n");
// }

void spin (uint32_t cnt) {
    for(uint32_t i = 0; i < cnt; i++) {
        asm("nop");
    }
}

void _other_function(void) {
    char buffer[128];
    int x;
    snprintf(buffer, sizeof(buffer), fmt_other, &x);
    uart_send(buffer);
    while(1) {
        uart_send("Other function\n");
        spin(1000000);
    }
}

void c_entry(void) {
    other_stack_end = other_stack + sizeof(other_stack);
    clock_init();
    enable_rccs();
    set_gpio_mode(GPIOB, 7, 1);
    uart_init();
    char buffer[128];
    {
        int x;
        snprintf(buffer, sizeof(buffer), fmt_init, &x);
        uart_send(buffer);
    }

    for(int j = 0; ; j++) {
        for(int i = 0; i < 1000000; i++);
        set_gpio_output(GPIOB, 7, 1);
        if(j == 10) {
            set_pendsv();
        }
        for(int i = 0; i < 1000000; i++);
        set_gpio_output(GPIOB, 7, 0);
    }
}
