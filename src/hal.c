#include "hal.h"

#define BIT(x) ((1ul) << (x))
#define FLASH_LATENCY 5
#define PLL_P 2
#define PLL_HSI 16
#define PLL_M 8
#define PLL_N 180
#define PLL_P 2
#define APB1_PRE 5
#define APB2_PRE 4
#define SYS_FREQUENCY ((PLL_HSI * PLL_N / PLL_M / PLL_P) * 1000000)
#define APB2_FREQUENCY (SYS_FREQUENCY / (BIT(APB2_PRE - 3)))
#define APB1_FREQUENCY (SYS_FREQUENCY / (BIT(APB1_PRE - 3)))

#if SYS_FREQUENCY > 180000000
#error "Clock frequency too high"
#endif

#if APB1_FREQUENCY > 45000000
#error "APB1 frequency too high"
#endif

#if APB2_FREQUENCY > 90000000
#error "APB2 frequency too high"
#endif

static struct {
    volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR,
             RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR,
             RESERVED2, APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR,
             AHB3LPENR, RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR, CSR,
             RESERVED6[2], SSCGR, PLLI2SCFGR;
} *RCC = (void *)(0x40023800);

static struct {
    volatile uint32_t ACR, KEYR, OPTKEYR, SR, CR, OPTCR;
} *FLASH = (void *)(0x40023C00);

void enable_rccs (void) {
    RCC->AHB1ENR |= 2; // Enable GPIOB RCC
}

void set_gpio_mode (gpio_t *periph, uint32_t pin, uint32_t state) {
    periph->MODER &= ~(3 << (pin * 2));
    periph->MODER |= state << (pin * 2);
}

void set_gpio_output (gpio_t *periph, uint32_t pin, bool state) {
    uint32_t state_bit = state ? 1 : 0;
    periph->ODR &= ~(1 << pin);
    periph->ODR |= state_bit << pin;
}

void clock_init (void) {
    FLASH->ACR |= FLASH_LATENCY | BIT(8) | BIT(9);     // Flash latency + prefetch + instruction cache
    RCC->PLLCFGR &= ~((BIT(17) - 1));                  // Clear PLL multipliers
    RCC->PLLCFGR |= (((PLL_P - 2) / 2) & 3) << 16;     // Set PLL_P
    RCC->PLLCFGR |= PLL_M | (PLL_N << 6);              // Set PLL_M and PLL_N
    RCC->CR |= BIT(24);                                // Enable PLL
    while ((RCC->CR & BIT(25)) == 0);
    RCC->CFGR = (APB1_PRE << 10) | (APB2_PRE << 13); // Set prescalers
    RCC->CFGR |= 2;                                  // Set clock source to PLL
    while ((RCC->CFGR & 12) == 0);

    // RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;   // Enable SYSCFG
    // SysTick_Config(SystemCoreClock / 1000); // Sys tick every 1ms
}
