#ifndef UART_H
#define UART_H

#include <stdint.h>

#define PERIPHERAL_BASE (0x40000000U)
#define AHB1_BASE (PERIPHERAL_BASE + 0x20000U)
#define GPIOB_BASE (AHB1_BASE + 0x400U)
#define RCC_BASE (AHB1_BASE + 0x3800U)

#define RCC_AHB1ENR_OFFSET (0x30U)
#define RCC_AHB1ENR ((volatile uint32_t*) (RCC_BASE + RCC_AHB1ENR_OFFSET))
#define RCC_AHB1ENR_GPIOAEN (0x00U)

#define GPIO_MODER_OFFSET (0x00U) 
#define GPIOB_MODER ((volatile uint32_t*) (GPIOB_BASE + GPIO_MODER_OFFSET))
#define GPIO_MODER_MODER5 (14U)
#define GPIO_ODR_OFFSET (0x14U)
#define GPIOB_ODR ((volatile uint32_t*) (GPIOB_BASE + GPIO_ODR_OFFSET))

#define LED_PIN 7


// UART:
#define GPIO_D_BASE (AHB1_BASE + 0x0C00U)
#define USART3_BASE (0x40004800U)

#define RCC_AHB1ENR_GPIODEN (3U)

#define RCC_APB1ENR_OFFSET (0x40U)
#define RCC_APB1ENR ((volatile uint32_t*) (RCC_BASE + RCC_APB1ENR_OFFSET))
#define RCC_APB1ENR_USART3EN (18U)

#define GPIOD_MODER_OFFSET (0x00U)
#define GPIOD_MODER ((volatile uint32_t*) (GPIO_D_BASE + GPIOD_MODER_OFFSET))
#define GPIOD_AFRH_OFFSET (0x24U)
#define GPIOD_AFRH ((volatile uint32_t*) (GPIO_D_BASE + GPIOD_AFRH_OFFSET))

#define GPIO_MODER_MODER8 (16U)
#define GPIO_MODER_MODER9 (18U)
#define GPIO_AFRH_AFRH8 (0U)
#define GPIO_AFRH_AFRH9 (4U)
#define GPIO_AF7_USART3 (7U)

#define USART3_CR1_OFFSET (0x00U)
#define USART3_CR1 ((volatile uint32_t*) (USART3_BASE + USART3_CR1_OFFSET))
#define USART3_BRR_OFFSET (0x0CU)
#define USART3_BRR ((volatile uint32_t*) (USART3_BASE + USART3_BRR_OFFSET))
#define USART3_ISR_OFFSET (0x1CU)
#define USART3_ISR ((volatile uint32_t*) (USART3_BASE + USART3_ISR_OFFSET))
#define USART3_RDR_OFFSET (0x24U)
#define USART3_RDR ((volatile uint32_t*) (USART3_BASE + USART3_RDR_OFFSET))
#define USART3_TDR_OFFSET (0x28U)
#define USART3_TDR ((volatile uint32_t*) (USART3_BASE + USART3_TDR_OFFSET))
#define USART3_ICR_OFFSET (0x20U)
#define USART3_ICR ((volatile uint32_t*) (USART3_BASE + USART3_ICR_OFFSET))

#define USART3_CR1_UE (0U)
#define USART3_CR1_RE (2U)
#define USART3_CR1_TE (3U)
#define USART3_ISR_ORE (3U)
#define USART3_ISR_RXNE (5U)
#define USART3_ISR_TXE (7U)
#define USART3_ICR_ORECF (3U)

// PCLK1 = 16 MHz (HSI, prescalers /1) -> BRR = 16000000 / 115200
#define USART3_BRR_115200 (139U)

void initUART(void);
void writeUART(char c);
char readUART(void);
void writeStrUART(const char *s);
uint32_t readStrUART(char *buf, uint32_t size);

#endif
