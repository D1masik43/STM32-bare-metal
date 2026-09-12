#include "ssd1306.h"
#include "i2c.h"

/* Every SSD1306 transfer is: address, one control byte, then payload.
 * 0x00 = the bytes that follow are commands, 0x40 = they are pixel data. */
#define OLED_CMD  (0x00U)
#define OLED_DATA (0x40U)

/* One byte covers 8 vertically stacked pixels (a "page"), so the buffer is
 * width x height/8 = 1024 bytes for a 128x64 panel. Lives in .bss. */
static uint8_t fb[OLED_WIDTH * OLED_PAGES];
uint8_t oledAddr;

static int oledCmd(uint8_t c)
{
  uint8_t b[2];
  b[0] = OLED_CMD;
  b[1] = c;
  return writeI2C(oledAddr, b, 2);
}

/* Init as ONE transaction: address, 0x00, then every command byte. That is
 * what the panel expects and avoids 25 separate transfers.
 * Returns 1 if the whole block was accepted at this address. */
static int tryInitAt(uint8_t a)
{
  static const uint8_t cmds[] = {
    0xAE,             /* display off                                  */
    0xD5, 0x80,       /* clock divide / oscillator frequency          */
    0xA8, OLED_HEIGHT - 1,  /* multiplex: 0x3F (64) or 0x1F (32)      */
    0xD3, 0x00,       /* display offset none                          */
    0x40,             /* start line 0                                 */
    0x8D, 0x14,       /* charge pump on - blank screen without this   */
    0x20, 0x00,       /* horizontal addressing                        */
    0xA1,             /* segment remap                                */
    0xC8,             /* COM scan descending                          */
    0xDA, (OLED_HEIGHT > 32U) ? 0x12 : 0x02,  /* COM pin layout       */
    0x81, 0xCF,       /* contrast                                     */
    0xD9, 0xF1,       /* pre-charge                                   */
    0xDB, 0x40,       /* VCOMH deselect                               */
    0xA4,             /* resume from RAM                              */
    0xA6,             /* not inverted                                 */
    0x2E,             /* scrolling off                                */
    0xAF,             /* display on                                   */
  };

  uint8_t buf[1 + sizeof(cmds)];

  buf[0] = OLED_CMD;
  for (uint32_t i = 0; i < sizeof(cmds); i++)
  {
    buf[1 + i] = cmds[i];
  }

  oledAddr = a;
  return (writeI2C(a, buf, sizeof(buf)) == 0) ? 1 : 0;
}

/* Do not trust probeI2C to pick the address - a noisy bus can fake an ACK.
 * Just try the real init at each and see which one actually takes. */
int initOLED(void)
{
  if (tryInitAt(0x3C)) { return 1; }
  if (tryInitAt(0x3D)) { return 1; }

  oledAddr = 0;
  return 0;
}

void clearOLED(void)
{
  for (uint32_t i = 0; i < sizeof(fb); i++)
  {
    fb[i] = 0;
  }
}

/* y/8 picks the page, y%8 the bit inside it. */
void drawPixelOLED(uint32_t x, uint32_t y, uint32_t on)
{
  if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
  {
    return;
  }

  if (on)
  {
    fb[x + (y / 8U) * OLED_WIDTH] |=  (uint8_t)(1U << (y % 8U));
  }
  else
  {
    fb[x + (y / 8U) * OLED_WIDTH] &= (uint8_t)~(1U << (y % 8U));
  }
}

void showOLED(void)
{
  oledCmd(0x21); oledCmd(0);              /* column range 0..127 */
  oledCmd(OLED_WIDTH - 1);
  oledCmd(0x22); oledCmd(0);              /* page range 0..7     */
  oledCmd(OLED_PAGES - 1);

  /* NBYTES in CR2 is only 8 bits, so a 1024-byte buffer cannot go in one
   * transfer. Horizontal addressing keeps advancing the panel's pointer
   * across transfers, so chunks of 32 are fine. */
  uint8_t chunk[33];
  chunk[0] = OLED_DATA;

  for (uint32_t i = 0; i < sizeof(fb); i += 32U)
  {
    for (uint32_t j = 0; j < 32U; j++)
    {
      chunk[1U + j] = fb[i + j];
    }

    writeI2C(oledAddr, chunk, 33);
  }
}
