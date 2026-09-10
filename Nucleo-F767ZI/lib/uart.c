#include "uart.h"

void initUART(void){

  *RCC_AHB1ENR |= (1 << RCC_AHB1ENR_GPIODEN);
  *RCC_APB1ENR |= (1 << RCC_APB1ENR_USART3EN);
 
  *GPIOD_MODER &= ~((3 << GPIO_MODER_MODER8) | (3 << GPIO_MODER_MODER9));
  *GPIOD_MODER |= (2 << GPIO_MODER_MODER8) | (2 << GPIO_MODER_MODER9);
 
  *GPIOD_AFRH &= ~((0xF << GPIO_AFRH_AFRH8) | (0xF << GPIO_AFRH_AFRH9));
  *GPIOD_AFRH |= (GPIO_AF7_USART3 << GPIO_AFRH_AFRH8) | (GPIO_AF7_USART3 << GPIO_AFRH_AFRH9);
 
  *USART3_CR1 = 0;
  *USART3_BRR = USART3_BRR_115200;

  *USART3_CR1 |= (1 << USART3_CR1_TE) | (1 << USART3_CR1_RE) | (1 << USART3_CR1_UE);

}

void writeUART(char c){
  while(!(*USART3_ISR & (1 << USART3_ISR_TXE)));
  *USART3_TDR = (uint32_t)c;
}

char readUART(void){
  while(!(*USART3_ISR & (1 << USART3_ISR_RXNE)))
  {
    if (*USART3_ISR & (1 << USART3_ISR_ORE))
    {
      *USART3_ICR = (1 << USART3_ICR_ORECF);
    }
  }
  return (char)(*USART3_RDR & 0xFF);
}

void writeStrUART(const char *s){
  while(*s)
  {
    writeUART(*s++);
  }
}

uint32_t readStrUART(char *buf, uint32_t size){
  uint32_t i = 0;

  while(1)
  {
    char c = readUART();

    if (c == '\r' || c == '\n')
    {
      break;
    }

    if (i + 1 < size)
    {
      buf[i++] = c;
    }
  }

  if (size > 0)
  {
    buf[i] = '\0';
  }

  return i;
}
