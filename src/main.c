#include <stdint.h>
#include <stdbool.h>
#include "hal.h"

void c_entry(void) {
    clock_init();
    enable_rccs();
    set_gpio_mode(GPIOB, 7, 1);
    uart_init();
    uart_send("Hello, world!\n");
    for(;;) {
        for(int i = 0; i < 1000000; i++);
        set_gpio_output(GPIOB, 7, 1);
        for(int i = 0; i < 1000000; i++);
        set_gpio_output(GPIOB, 7, 0);
    }
}
