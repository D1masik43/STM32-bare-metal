#include <stdint.h>
#include "lib/uart.h"
#include "lib/i2c.h"
#include "lib/ssd1306.h"

// LD2 (blue) on PB7
#define LED_PIN     (7U)
#define GPIOB_MODER GPIO_MODER(GPIOB_BASE)
#define GPIOB_ODR   GPIO_ODR(GPIOB_BASE)

// Border around the panel plus a big X, drawn one pixel at a time.
static void drawDemo(void)
{
  clearOLED();

  for (uint32_t x = 0; x < OLED_WIDTH; x++)
  {
    drawPixelOLED(x, 0, 1);
    drawPixelOLED(x, OLED_HEIGHT - 1, 1);
  }

  for (uint32_t y = 0; y < OLED_HEIGHT; y++)
  {
    drawPixelOLED(0, y, 1);
    drawPixelOLED(OLED_WIDTH - 1, y, 1);
  }

  // two diagonals: x steps 128/64 = 2 per row
  for (uint32_t y = 0; y < OLED_HEIGHT; y++)
  {
    uint32_t x = (y * OLED_WIDTH) / OLED_HEIGHT;

    drawPixelOLED(x, y, 1);
    drawPixelOLED(OLED_WIDTH - 1 - x, y, 1);
  }

  showOLED();
}

// Read SCL/SDA as plain inputs before I2C takes them over. Both must idle
// HIGH - a 0 means something is holding that line down and no I2C can work.
static void checkI2CLines(void)
{
  *RCC_AHB1ENR |= (1 << RCC_AHB1ENR_GPIOBEN);

  *GPIO_MODER(GPIOB_BASE) &= ~((3U << (8U * 2U)) | (3U << (9U * 2U)));  // input
  *GPIO_PUPDR(GPIOB_BASE) &= ~((3U << (8U * 2U)) | (3U << (9U * 2U)));
  *GPIO_PUPDR(GPIOB_BASE) |=  ((1U << (8U * 2U)) | (1U << (9U * 2U)));  // pull-up

  for (volatile uint32_t i = 0; i < 50000U; i++) { }   // let them settle

  uint32_t idr = *GPIO_IDR(GPIOB_BASE);

  writeStrUART("SCL(PB8)=");
  writeUART((idr & (1U << 8U)) ? '1' : '0');
  writeStrUART("  SDA(PB9)=");
  writeUART((idr & (1U << 9U)) ? '1' : '0');
  writeStrUART("   (both must be 1)\r\n");
}

// Report every address that acknowledges. Decimal, so an SSD1306 at 0x3C
// shows up as 60 and 0x3D as 61.
static void scanI2C(void)
{
  char num[11];
  uint32_t found = 0;

  writeStrUART("scanning I2C...\r\n");

  for (uint8_t a = 1; a < 128; a++)
  {
    if (probeI2C(a))
    {
      decToStr(a, num);
      writeStrUART("  found device at ");
      writeStrUART(num);
      writeStrUART("\r\n");
      found++;
    }
  }

  decToStr(found, num);
  writeStrUART("scan done, ");
  writeStrUART(num);
  writeStrUART(" device(s)\r\n");
}

void main(void)
{
  *RCC_AHB1ENR |= (1 << RCC_AHB1ENR_GPIOBEN);

  *GPIOB_MODER |= (1 << (LED_PIN * 2));   // output

  initUART();

  checkI2CLines();

  initI2C();

  char line[64];

  writeStrUART("\r\nNucleo-F767ZI ready\r\n");

  scanI2C();

  if (initOLED())
  {
    char a[11];
    decToStr(oledAddr, a);
    writeStrUART("OLED at ");
    writeStrUART(a);
    writeStrUART(", drawing\r\n");
    drawDemo();
  }
  else
  {
    char n[11];
    writeStrUART("no OLED - i2c err ");
    decToStr(i2cErrCode, n); writeStrUART(n);
    writeStrUART(" at byte ");
    decToStr(i2cErrByte, n); writeStrUART(n);
    writeStrUART(" ISR=");
    decToStr(i2cErrISR, n);  writeStrUART(n);
    writeStrUART("\r\n  (1=busy 2=NACK 3=stall 4=noSTOP)\r\n");
  }

  // Lab 1 exchange model: read a packet terminated by \r and answer
  // ACK:<message>\n. Nothing else goes out once the banner is done, so the
  // host's read_until('\n') sees the acknowledgement and nothing else.
  while(1)
  {
    if (readStrUART(line, sizeof(line)))
    {
      *GPIOB_ODR ^= (1 << LED_PIN);

      writeStrUART("ACK:");
      writeStrUART(line);
      writeUART('\n');
    }

  }

}
