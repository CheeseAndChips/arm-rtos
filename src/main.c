#include <stdint.h>
#include <stdbool.h>

struct {
    volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR,
             RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR,
             RESERVED2, APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR,
             AHB3LPENR, RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR, CSR,
             RESERVED6[2], SSCGR, PLLI2SCFGR;
} *RCC = (void *)(0x40023800);

typedef struct {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFRL, AFRH;
} gpio_t;

#define GPIO_BASE 0x40020000
gpio_t *GPIOA = (void *)(GPIO_BASE + 0x400 * 0);
gpio_t *GPIOB = (void *)(GPIO_BASE + 0x400 * 1);

void set_gpio_mode (gpio_t *periph, uint32_t pin, uint32_t state) {
    periph->MODER &= ~(3 << (pin * 2));
    periph->MODER |= state << (pin * 2);
}

void set_gpio_output (gpio_t *periph, uint32_t pin, bool state) {
    uint32_t state_bit = state ? 1 : 0;
    periph->ODR &= ~(1 << pin);
    periph->ODR |= state_bit << pin;
}

void c_entry(void) {
    RCC->AHB1ENR |= 2; // Enable GPIOB RCC
    set_gpio_mode(GPIOB, 7, 1);
    for(;;) {
        for(int i = 0; i < 1000000; i++);
        set_gpio_output(GPIOB, 7, 1);
        for(int i = 0; i < 1000000; i++);
        set_gpio_output(GPIOB, 7, 0);
    }
}
