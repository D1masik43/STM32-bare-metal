#include <stdint.h>
#include "lib/uart.h"

void main(void)
{
  *RCC_AHB1ENR |= (2 << RCC_AHB1ENR_GPIOAEN);

  *GPIOB_MODER |= (1 << GPIO_MODER_MODER5);

  initUART();

  char line[64];

  writeStrUART("\r\nNucleo-F767ZI ready\r\n");

  while(1)
  {
    writeStrUART("> ");
    readStrUART(line, sizeof(line));

    *GPIOB_ODR ^= (1 << LED_PIN);

    writeStrUART("you typed: ");
    writeStrUART(line);
    writeStrUART("\r\n");
  }

}
