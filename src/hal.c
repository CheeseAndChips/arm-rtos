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

#define UART_TX 8 // on PORTD
#define UART_RX 9 // on PORTD
#define BAUDRATE 115200

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

static struct {
    volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR;
} *USART3 = (void *)(0x40004800);

struct {
    uint32_t CPUID, ICSR;
    // ...
} *SCB = (void*)(0xE000ED00);

void set_pendsv (void) {
    SCB->ICSR |= BIT(28);
}

void enable_rccs (void) {
    RCC->AHB1ENR |= BIT(1);  // Enable GPIOB RCC
    RCC->APB1ENR |= BIT(18); // Enable USART3 RCC
    RCC->AHB1ENR |= BIT(3);  // Enable GPIOD RCC
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

static void set_gpio_alternate_function (gpio_t *periph, uint32_t pin, uint32_t function) {
    if (pin < 8) {
        periph->AFRL &= ~(0xF << (pin * 4));
        periph->AFRL |= function << (pin * 4);
    } else {
        periph->AFRH &= ~(0xF << ((pin - 8) * 4));
        periph->AFRH |= function << ((pin - 8) * 4);
    }
}

void uart_init (void) {
    set_gpio_mode(GPIOD, UART_TX, 0b10); // alternate function
    set_gpio_mode(GPIOD, UART_RX, 0b10); // alternate function
    set_gpio_alternate_function(GPIOD, UART_TX, 7); // USART3 TX AF
    set_gpio_alternate_function(GPIOD, UART_RX, 7); // USART3 RX AF
    USART3->CR1 = 0;
    USART3->BRR = APB1_FREQUENCY / BAUDRATE;
    USART3->CR1 = BIT(3) | BIT(2) | BIT(13); // TE, RE, UE
}

void uart_send (const char *str) {
    while (*str) {
        while ((USART3->SR & BIT(7)) == 0);
        USART3->DR = *str++;
    }
}

void clock_init (void) {
    FLASH->ACR |= FLASH_LATENCY | BIT(8) | BIT(9);      // Flash latency + prefetch + instruction cache
    RCC->PLLCFGR &= ~((BIT(17) - 1));                   // Clear PLL multipliers
    RCC->PLLCFGR |= (((PLL_P - 2) / 2) & 3) << 16;      // Set PLL_P
    RCC->PLLCFGR |= PLL_M | (PLL_N << 6);               // Set PLL_M and PLL_N
    RCC->CR |= BIT(24);                                 // Enable PLL
    while ((RCC->CR & BIT(25)) == 0);
    RCC->CFGR = (APB1_PRE << 10) | (APB2_PRE << 13);    // Set prescalers
    RCC->CFGR |= 2;                                     // Set clock source to PLL
    while ((RCC->CFGR & 12) == 0);

    // RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;   // Enable SYSCFG
    // SysTick_Config(SystemCoreClock / 1000); // Sys tick every 1ms
}
