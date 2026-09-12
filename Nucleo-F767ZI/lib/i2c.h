#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "mcu.h"

#define I2C1_BASE (0x40005400U)

#define I2C1_CR1      ((volatile uint32_t*) (I2C1_BASE + 0x00U))
#define I2C1_CR2      ((volatile uint32_t*) (I2C1_BASE + 0x04U))
#define I2C1_OAR1     ((volatile uint32_t*) (I2C1_BASE + 0x08U))
#define I2C1_OAR2     ((volatile uint32_t*) (I2C1_BASE + 0x0CU))
#define I2C1_TIMINGR  ((volatile uint32_t*) (I2C1_BASE + 0x10U))
#define I2C1_TIMEOUTR ((volatile uint32_t*) (I2C1_BASE + 0x14U))
#define I2C1_ISR      ((volatile uint32_t*) (I2C1_BASE + 0x18U))
#define I2C1_ICR      ((volatile uint32_t*) (I2C1_BASE + 0x1CU))
#define I2C1_PECR     ((volatile uint32_t*) (I2C1_BASE + 0x20U))
#define I2C1_RXDR     ((volatile uint32_t*) (I2C1_BASE + 0x24U))
#define I2C1_TXDR     ((volatile uint32_t*) (I2C1_BASE + 0x28U))

/* CR1 */
#define I2C_CR1_PE        (0U)   /* peripheral enable; clearing it resets the FSM */
#define I2C_CR1_TXIE      (1U)
#define I2C_CR1_RXIE      (2U)
#define I2C_CR1_ADDRIE    (3U)
#define I2C_CR1_NACKIE    (4U)
#define I2C_CR1_STOPIE    (5U)
#define I2C_CR1_TCIE      (6U)
#define I2C_CR1_ERRIE     (7U)
#define I2C_CR1_DNF       (8U)   /* [11:8] digital noise filter */
#define I2C_CR1_ANFOFF    (12U)  /* analog filter OFF (0 = filter on, the default) */
#define I2C_CR1_SBC       (16U)
#define I2C_CR1_NOSTRETCH (17U)  /* must stay 0 in master mode */

/* CR2 - the transfer is described here, then START launches it */
#define I2C_CR2_SADD      (0U)   /* [9:0]  7-bit address goes in bits 7:1 */
#define I2C_CR2_RD_WRN    (10U)  /* 0 = write, 1 = read */
#define I2C_CR2_ADD10     (11U)
#define I2C_CR2_HEAD10R   (12U)
#define I2C_CR2_START     (13U)  /* self-clearing once the address is on the bus */
#define I2C_CR2_STOP      (14U)
#define I2C_CR2_NACK      (15U)
#define I2C_CR2_NBYTES    (16U)  /* [23:16] how many bytes this transfer moves */
#define I2C_CR2_RELOAD    (24U)  /* needed only for transfers > 255 bytes */
#define I2C_CR2_AUTOEND   (25U)  /* hardware sends STOP after NBYTES */
#define I2C_CR2_PECBYTE   (26U)

/* ISR */
#define I2C_ISR_TXE       (0U)   /* TXDR empty */
#define I2C_ISR_TXIS      (1U)   /* ready for the next byte to transmit */
#define I2C_ISR_RXNE      (2U)   /* a received byte is waiting in RXDR */
#define I2C_ISR_ADDR      (3U)   /* slave mode only */
#define I2C_ISR_NACKF     (4U)   /* slave did not acknowledge */
#define I2C_ISR_STOPF     (5U)   /* STOP seen on the bus */
#define I2C_ISR_TC        (6U)   /* transfer complete, AUTOEND = 0 */
#define I2C_ISR_TCR       (7U)   /* transfer complete reload */
#define I2C_ISR_BERR      (8U)
#define I2C_ISR_ARLO      (9U)   /* arbitration lost (multi-master) */
#define I2C_ISR_OVR       (10U)
#define I2C_ISR_PECERR    (11U)
#define I2C_ISR_TIMEOUT   (12U)
#define I2C_ISR_ALERT     (13U)
#define I2C_ISR_BUSY      (15U)  /* bus is not idle */
#define I2C_ISR_DIR       (16U)
#define I2C_ISR_ADDCODE   (17U)  /* [23:17] */

/* ICR - write 1 to clear the matching ISR flag */
#define I2C_ICR_ADDRCF    (3U)
#define I2C_ICR_NACKCF    (4U)
#define I2C_ICR_STOPCF    (5U)
#define I2C_ICR_BERRCF    (8U)
#define I2C_ICR_ARLOCF    (9U)
#define I2C_ICR_OVRCF     (10U)
#define I2C_ICR_PECCF     (11U)
#define I2C_ICR_TIMOUTCF  (12U)
#define I2C_ICR_ALERTCF   (13U)

/* TIMINGR field positions */
#define I2C_TIMINGR_SCLL   (0U)   /* [7:0]   */
#define I2C_TIMINGR_SCLH   (8U)   /* [15:8]  */
#define I2C_TIMINGR_SDADEL (16U)  /* [19:16] */
#define I2C_TIMINGR_SCLDEL (20U)  /* [23:20] */
#define I2C_TIMINGR_PRESC  (28U)  /* [31:28] */

#define I2C_TIMING_100KHZ_16MHZ (0x30420F13U)
#define I2C_TIMING_400KHZ_16MHZ (0x10320309U)

#define I2C_SPIN_TIMEOUT (20000U)   /* ~7ms; a 100kHz frame is ~90us */

/* Why the last writeI2C/readI2C failed:
 * 0 = ok, 1 = bus stayed busy, 2 = NACK, 3 = stalled mid-byte, 4 = no STOP.
 * i2cErrISR is I2C1_ISR captured at the moment of failure. */
extern uint32_t i2cErrCode;
extern uint32_t i2cErrISR;
extern uint32_t i2cErrByte;

void initI2C(void);
int  writeI2C(uint8_t addr, const uint8_t *data, uint32_t len);
int  readI2C(uint8_t addr, uint8_t *data, uint32_t len);
int  probeI2C(uint8_t addr);

#endif
