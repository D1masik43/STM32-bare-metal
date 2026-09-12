#include "i2c.h"

#define I2C_PORT    GPIOB_BASE
#define I2C_PORT_EN RCC_AHB1ENR_GPIOBEN
#define I2C_SCL_PIN (8U)
#define I2C_SDA_PIN (9U)
#define I2C_AF      (4U)

#define I2C_WAIT(cond)                          \
  do {                                          \
    uint32_t _t = I2C_SPIN_TIMEOUT;             \
    while (!(cond))                             \
    {                                           \
      if (--_t == 0U) { return -1; }            \
    }                                           \
  } while (0)

void initI2C(void)
{
  *RCC_AHB1ENR |= (1U << I2C_PORT_EN);
  *RCC_APB1ENR |= (1U << RCC_APB1ENR_I2C1EN);

  *GPIO_MODER(I2C_PORT) &= ~((3U << (I2C_SCL_PIN * 2U)) | (3U << (I2C_SDA_PIN * 2U)));
  *GPIO_MODER(I2C_PORT) |=  ((GPIO_MODE_AF << (I2C_SCL_PIN * 2U)) |
                             (GPIO_MODE_AF << (I2C_SDA_PIN * 2U)));

  *GPIO_OTYPER(I2C_PORT) |= (1U << I2C_SCL_PIN) | (1U << I2C_SDA_PIN);

  *GPIO_PUPDR(I2C_PORT) &= ~((3U << (I2C_SCL_PIN * 2U)) | (3U << (I2C_SDA_PIN * 2U)));
  *GPIO_PUPDR(I2C_PORT) |=  ((GPIO_PULL_UP << (I2C_SCL_PIN * 2U)) |
                             (GPIO_PULL_UP << (I2C_SDA_PIN * 2U)));

  *GPIO_AFRH(I2C_PORT) &= ~((0xFU << ((I2C_SCL_PIN - 8U) * 4U)) |
                            (0xFU << ((I2C_SDA_PIN - 8U) * 4U)));
  *GPIO_AFRH(I2C_PORT) |=  ((I2C_AF << ((I2C_SCL_PIN - 8U) * 4U)) |
                            (I2C_AF << ((I2C_SDA_PIN - 8U) * 4U)));

  *I2C1_CR1 = 0;
  *I2C1_TIMINGR = I2C_TIMING_100KHZ_16MHZ;
  *I2C1_CR1 |= (1U << I2C_CR1_PE);
}

static void i2cStart(uint8_t addr, uint32_t len, uint32_t read)
{
  *I2C1_CR2 = (((uint32_t)addr << 1) & 0x3FEU)
            | (len << I2C_CR2_NBYTES)
            | (read ? (1U << I2C_CR2_RD_WRN) : 0U)
            | (1U << I2C_CR2_AUTOEND)
            | (1U << I2C_CR2_START);
}

static void i2cClearFlags(void)
{
  *I2C1_ICR = (1U << I2C_ICR_NACKCF) | (1U << I2C_ICR_STOPCF) |
              (1U << I2C_ICR_BERRCF) | (1U << I2C_ICR_ARLOCF);
}

uint32_t i2cErrCode;
uint32_t i2cErrISR;
uint32_t i2cErrByte;

static int i2cFail(uint32_t code, uint32_t byte)
{
  i2cErrCode = code;
  i2cErrISR  = *I2C1_ISR;
  i2cErrByte = byte;
  i2cClearFlags();
  return -1;
}


static void i2cReset(void)
{
  *I2C1_CR1 &= ~(1U << I2C_CR1_PE);
  while (*I2C1_CR1 & (1U << I2C_CR1_PE)) { }
  *I2C1_CR1 |= (1U << I2C_CR1_PE);
}


static int i2cWaitIdle(void)
{
  uint32_t t = I2C_SPIN_TIMEOUT;

  while (*I2C1_ISR & (1U << I2C_ISR_BUSY))
  {
    if (--t == 0U)
    {
      i2cReset();

      t = I2C_SPIN_TIMEOUT;
      while (*I2C1_ISR & (1U << I2C_ISR_BUSY))
      {
        if (--t == 0U) { return -1; }
      }
      return 0;
    }
  }

  return 0;
}

static void i2cFinishStop(void)
{
  uint32_t t = I2C_SPIN_TIMEOUT;

  while (!(*I2C1_ISR & (1U << I2C_ISR_STOPF)))
  {
    if (--t == 0U) { break; }
  }

  i2cClearFlags();
}

int writeI2C(uint8_t addr, const uint8_t *data, uint32_t len)
{
  if (i2cWaitIdle() != 0) { return i2cFail(1, 0); }

  i2cStart(addr, len, 0U);

  for (uint32_t i = 0; i < len; i++)
  {
    uint32_t t = I2C_SPIN_TIMEOUT;

    while (!(*I2C1_ISR & ((1U << I2C_ISR_TXIS) | (1U << I2C_ISR_NACKF))))
    {
      if (--t == 0U) { return i2cFail(3, i); }
    }

    if (*I2C1_ISR & (1U << I2C_ISR_NACKF))
    {
      i2cErrCode = 2; i2cErrISR = *I2C1_ISR; i2cErrByte = i;
      i2cFinishStop();
      return -1;
    }

    *I2C1_TXDR = data[i];
  }

  uint32_t t = I2C_SPIN_TIMEOUT;
  while (!(*I2C1_ISR & (1U << I2C_ISR_STOPF)))
  {
    if (--t == 0U) { return i2cFail(4, len); }
  }

  i2cErrCode = 0;
  i2cClearFlags();
  return 0;
}

int readI2C(uint8_t addr, uint8_t *data, uint32_t len)
{
  if (i2cWaitIdle() != 0) { return i2cFail(1, 0); }

  i2cStart(addr, len, 1U);

  for (uint32_t i = 0; i < len; i++)
  {
    uint32_t t = I2C_SPIN_TIMEOUT;

    while (!(*I2C1_ISR & ((1U << I2C_ISR_RXNE) | (1U << I2C_ISR_NACKF))))
    {
      if (--t == 0U) { return i2cFail(3, i); }
    }

    if (*I2C1_ISR & (1U << I2C_ISR_NACKF))
    {
      i2cErrCode = 2; i2cErrISR = *I2C1_ISR; i2cErrByte = i;
      i2cFinishStop();
      return -1;
    }

    data[i] = (uint8_t)(*I2C1_RXDR & 0xFFU);
  }

  uint32_t t = I2C_SPIN_TIMEOUT;
  while (!(*I2C1_ISR & (1U << I2C_ISR_STOPF)))
  {
    if (--t == 0U) { return i2cFail(4, len); }
  }

  i2cErrCode = 0;
  i2cClearFlags();
  return 0;
}

int probeI2C(uint8_t addr)
{
  if (i2cWaitIdle() != 0) { return 0; }

  i2cStart(addr, 0U, 0U);

  uint32_t t = I2C_SPIN_TIMEOUT;
  while (!(*I2C1_ISR & ((1U << I2C_ISR_STOPF) | (1U << I2C_ISR_NACKF))))
  {
    if (--t == 0U) { i2cClearFlags(); return 0; }
  }

  int acked = (*I2C1_ISR & (1U << I2C_ISR_NACKF)) ? 0 : 1;

  i2cFinishStop();   // do not leave a STOP in flight - it wedges BUSY
  return acked;
}
