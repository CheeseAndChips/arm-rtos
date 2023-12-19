#include "timer.h"

static struct {
    timer_t *timer;
    uint32_t period;
} rtc;

void timer_init(timer_t *timer) {
    timer->control |= TIMER_ENABLE | TIMER_32BIT | (1 << 5);
}

void timer_setup_callback(timer_t *timer, uint32_t period, void (*callback)(void)) {
    timer_init(timer);
}

extern void print_uart0(const char *s);
void c_irq_handler(void) {
    print_uart0("Interrupt!\n");
    while(1);
}
