#include "timer.h"

volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

void print_uart0(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
        *UART0DR = (unsigned int)(*s); /* Transmit char */
        s++; /* Next char */
    }
}

void c_entry() {
    timer_init(TIMER0);
    *(uint32_t*)(0x10140010) |= (1 << 4);
    uint32_t init_val = TIMER0->value;

    while(1) {
        print_uart0("Hello world from main!\n");
        for(uint32_t i = 0; i < 100000000; i++);
        while(init_val == TIMER0->value)
            ;
        //timer_delay_us(TIMER0, 1000000);
    }
}
