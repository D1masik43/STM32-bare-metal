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

void main(void)
{
  *RCC_AHB1ENR |= (2 << RCC_AHB1ENR_GPIOAEN);

  // do two dummy reads after enabling the peripheral clock, as per the errata
  volatile uint32_t dummy;
  dummy = *(RCC_AHB1ENR);
  dummy = *(RCC_AHB1ENR);

  *GPIOB_MODER |= (1 << GPIO_MODER_MODER5);
  
  while(1)
  {
    *GPIOB_ODR ^= (1 << LED_PIN);
    for (uint32_t i = 0; i < 1000000; i++);
  }

}
