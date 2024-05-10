#include <stdint.h>
#include <stdbool.h>

typedef struct {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFRL, AFRH;
} gpio_t;

#define GPIO_BASE 0x40020000
#define GPIOA ((gpio_t *)(GPIO_BASE + 0x400 * 0))
#define GPIOB ((gpio_t *)(GPIO_BASE + 0x400 * 1))

void enable_rccs (void);
void set_gpio_mode (gpio_t *periph, uint32_t pin, uint32_t state);
void set_gpio_output (gpio_t *periph, uint32_t pin, bool state);
void clock_init (void);
