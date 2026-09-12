#ifndef MCU_H
#define MCU_H

#include <stdint.h>

/* Shared MCU definitions for STM32F76x/F77x (RM0410 Rev 5).
 * Bus bases, RCC clock gating, and GPIO. Peripheral drivers include this. */

/* ---- bus bases ---- */
#define PERIPHERAL_BASE (0x40000000U)
#define APB1_BASE       (PERIPHERAL_BASE + 0x00000U)
#define APB2_BASE       (PERIPHERAL_BASE + 0x10000U)
#define AHB1_BASE       (PERIPHERAL_BASE + 0x20000U)

/* ---- RCC ---- */
#define RCC_BASE    (AHB1_BASE + 0x3800U)
#define RCC_AHB1ENR ((volatile uint32_t*) (RCC_BASE + 0x30U))
#define RCC_APB1ENR ((volatile uint32_t*) (RCC_BASE + 0x40U))
#define RCC_APB2ENR ((volatile uint32_t*) (RCC_BASE + 0x44U))

/* AHB1ENR: GPIO port clock enable. Bit number == port index (A=0 .. K=10). */
#define RCC_AHB1ENR_GPIOAEN (0U)
#define RCC_AHB1ENR_GPIOBEN (1U)
#define RCC_AHB1ENR_GPIOCEN (2U)
#define RCC_AHB1ENR_GPIODEN (3U)
#define RCC_AHB1ENR_GPIOEEN (4U)
#define RCC_AHB1ENR_GPIOFEN (5U)
#define RCC_AHB1ENR_GPIOGEN (6U)
#define RCC_AHB1ENR_GPIOHEN (7U)
#define RCC_AHB1ENR_GPIOIEN (8U)
#define RCC_AHB1ENR_GPIOJEN (9U)
#define RCC_AHB1ENR_GPIOKEN (10U)

/* APB1ENR */
#define RCC_APB1ENR_USART2EN (17U)
#define RCC_APB1ENR_USART3EN (18U)
#define RCC_APB1ENR_UART4EN  (19U)
#define RCC_APB1ENR_UART5EN  (20U)
#define RCC_APB1ENR_I2C1EN   (21U)
#define RCC_APB1ENR_I2C2EN   (22U)
#define RCC_APB1ENR_I2C3EN   (23U)
#define RCC_APB1ENR_I2C4EN   (24U)   /* gap between I2C3EN(23) and CAN1EN(25) */
#define RCC_APB1ENR_UART7EN  (30U)
#define RCC_APB1ENR_UART8EN  (31U)

/* APB2ENR */
#define RCC_APB2ENR_USART1EN (4U)
#define RCC_APB2ENR_USART6EN (5U)

/* ---- GPIO ports ---- */
#define GPIOA_BASE (AHB1_BASE + 0x0000U)
#define GPIOB_BASE (AHB1_BASE + 0x0400U)
#define GPIOC_BASE (AHB1_BASE + 0x0800U)
#define GPIOD_BASE (AHB1_BASE + 0x0C00U)
#define GPIOE_BASE (AHB1_BASE + 0x1000U)
#define GPIOF_BASE (AHB1_BASE + 0x1400U)
#define GPIOG_BASE (AHB1_BASE + 0x1800U)
#define GPIOH_BASE (AHB1_BASE + 0x1C00U)
#define GPIOI_BASE (AHB1_BASE + 0x2000U)
#define GPIOJ_BASE (AHB1_BASE + 0x2400U)
#define GPIOK_BASE (AHB1_BASE + 0x2800U)

/* GPIO registers, taking a port base above. */
#define GPIO_MODER(p)   ((volatile uint32_t*) ((p) + 0x00U))  /* 2 bits per pin */
#define GPIO_OTYPER(p)  ((volatile uint32_t*) ((p) + 0x04U))  /* 1 bit  per pin */
#define GPIO_OSPEEDR(p) ((volatile uint32_t*) ((p) + 0x08U))  /* 2 bits per pin */
#define GPIO_PUPDR(p)   ((volatile uint32_t*) ((p) + 0x0CU))  /* 2 bits per pin */
#define GPIO_IDR(p)     ((volatile uint32_t*) ((p) + 0x10U))  /* 1 bit  per pin */
#define GPIO_ODR(p)     ((volatile uint32_t*) ((p) + 0x14U))  /* 1 bit  per pin */
#define GPIO_BSRR(p)    ((volatile uint32_t*) ((p) + 0x18U))
#define GPIO_AFRL(p)    ((volatile uint32_t*) ((p) + 0x20U))  /* 4 bits, pins 0-7  */
#define GPIO_AFRH(p)    ((volatile uint32_t*) ((p) + 0x24U))  /* 4 bits, pins 8-15 */

/* MODER field values */
#define GPIO_MODE_INPUT  (0U)
#define GPIO_MODE_OUTPUT (1U)
#define GPIO_MODE_AF     (2U)
#define GPIO_MODE_ANALOG (3U)

/* OTYPER field values */
#define GPIO_OTYPE_PUSHPULL  (0U)
#define GPIO_OTYPE_OPENDRAIN (1U)

/* PUPDR field values */
#define GPIO_PULL_NONE (0U)
#define GPIO_PULL_UP   (1U)
#define GPIO_PULL_DOWN (2U)

#endif
