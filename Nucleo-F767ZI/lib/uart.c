#include "uart.h"

void initUART(void){

  *RCC_AHB1ENR |= (1 << RCC_AHB1ENR_GPIODEN);
  *RCC_APB1ENR |= (1 << RCC_APB1ENR_USART3EN);
 
  *GPIOD_MODER &= ~((3 << GPIO_MODER_MODER8) | (3 << GPIO_MODER_MODER9));
  *GPIOD_MODER |= (2 << GPIO_MODER_MODER8) | (2 << GPIO_MODER_MODER9);
 
  *GPIOD_AFRH &= ~((0xF << GPIO_AFRH_AFRH8) | (0xF << GPIO_AFRH_AFRH9));
  *GPIOD_AFRH |= (GPIO_AF7_USART3 << GPIO_AFRH_AFRH8) | (GPIO_AF7_USART3 << GPIO_AFRH_AFRH9);
 
  *USART3_CR1 = 0;
  *USART3_BRR = USART3_BRR_38400;

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

// Polls on its own: drains whatever has arrived and returns 1 only once a
// full line is in buf. Returns 0 straight away while the line is still being
// typed, so the caller's loop keeps running.
int readStrUART(char *buf, uint32_t size){
  static uint32_t i = 0;

  // a latched overrun stops RXNE coming back, so clear it first
  if (*USART3_ISR & (1 << USART3_ISR_ORE))
  {
    *USART3_ICR = (1 << USART3_ICR_ORECF);
  }

  while (*USART3_ISR & (1 << USART3_ISR_RXNE))
  {
    char c = (char)(*USART3_RDR & 0xFF);

    if (c == '\r' || c == '\n')
    {
      if (size > 0)
      {
        buf[i] = '\0';
      }
      i = 0;
      return 1;
    }

    if (i + 1 < size)
    {
      buf[i++] = c;
    }
  }

  return 0;
}

// Decimal number -> string. buf needs room for 11 chars (10 digits + '\0').
void decToStr(uint32_t v, char *buf){
  char tmp[10];
  uint32_t n = 0;
  uint32_t i = 0;

  if (v == 0)
  {
    buf[0] = '0';
    buf[1] = '\0';
    return;
  }

  // %10 gives the lowest digit first, so collect then reverse
  while (v > 0)
  {
    tmp[n++] = (char)('0' + (v % 10));
    v /= 10;
  }

  while (n > 0)
  {
    buf[i++] = tmp[--n];
  }

  buf[i] = '\0';
}
