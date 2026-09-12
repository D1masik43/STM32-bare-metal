#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

/* Adafruit / generic SSD1306 OLED over I2C.
 * Set OLED_HEIGHT to 32 for the short 128x32 modules. */
#define OLED_WIDTH  (128U)
#define OLED_HEIGHT (64U)
#define OLED_PAGES  (OLED_HEIGHT / 8U)

extern uint8_t oledAddr;   // address that worked, 0 if none
int  initOLED(void);                                   // 1 = found and started
void clearOLED(void);                                  // blanks the buffer only
void drawPixelOLED(uint32_t x, uint32_t y, uint32_t on);
void showOLED(void);                                   // buffer -> panel

#endif
