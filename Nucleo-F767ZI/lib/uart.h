#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "mcu.h"

// UART:
#define USART3_BASE (0x40004800U)

#define GPIOD_MODER GPIO_MODER(GPIOD_BASE)
#define GPIOD_AFRH  GPIO_AFRH(GPIOD_BASE)

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
int  readStrUART(char *buf, uint32_t size);   // 1 = line ready, 0 = not yet
void decToStr(uint32_t v, char *buf);         // buf must hold 11 chars

#endif
