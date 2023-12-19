#ifndef _TIMER_H
#define _TIMER_H
#include <stdint.h>

enum {
    TIMER_ENABLE = 1 << 7,
    TIMER_32BIT = 1 << 1,
};

typedef __attribute__((__packed__)) struct {
    volatile uint32_t load;
    volatile uint32_t value;
    volatile uint32_t control;
    volatile uint32_t intclr;
    volatile uint32_t ris;
    volatile uint32_t mis;
    volatile uint32_t bgload;
} timer_t;

#define TIMER0 ((timer_t *) 0x101E2000)

void timer_init(timer_t *timer);
void timer_setup_callback(timer_t *timer, uint32_t period, void (*callback)(void));

#endif
