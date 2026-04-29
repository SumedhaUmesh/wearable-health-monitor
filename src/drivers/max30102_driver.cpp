/*
 * MAX30102 — register-oriented skeleton (datasheet Table 8 / register map summary).
 * I2C is intentionally not called here so firmware still links and runs without a bus.
 */

#include "max30102_driver.h"

#include <errno.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(max30102, LOG_LEVEL_DBG);

/* 7-bit address with ADDR → GND (see datasheet “I2C Address Selection”). */
static constexpr uint8_t kMax30102I2cAddr7 = 0x57U;

/* ---- Registers (why they matter for later bring-up) ---- */
static constexpr uint8_t REG_INTR_STATUS_1 = 0x00U; /* FIFO almost full / new data */
static constexpr uint8_t REG_INTR_STATUS_2 = 0x01U;
static constexpr uint8_t REG_INTR_ENABLE_1 = 0x02U;
static constexpr uint8_t REG_INTR_ENABLE_2 = 0x03U;
static constexpr uint8_t REG_FIFO_WR_PTR = 0x04U; /* FIFO write pointer */
static constexpr uint8_t REG_OVF_COUNTER = 0x05U; /* overflow */
static constexpr uint8_t REG_FIFO_RD_PTR = 0x06U;
static constexpr uint8_t REG_FIFO_DATA = 0x07U; /* burst read PPG/IR/RED samples */
static constexpr uint8_t REG_FIFO_CONFIG = 0x08U; /* sample averaging, FIFO rollover */
static constexpr uint8_t REG_MODE_CONFIG = 0x09U; /* HR only vs SpO2 vs multi-LED */
static constexpr uint8_t REG_SPO2_CONFIG = 0x0AU; /* ADC range, sample rate, LED width */
static constexpr uint8_t REG_LED1_PA = 0x0CU; /* IR LED current */
static constexpr uint8_t REG_LED2_PA = 0x0DU; /* Red LED current */
static constexpr uint8_t REG_PILOT_PA = 0x10U;
static constexpr uint8_t REG_MULTI_LED_CTRL1 = 0x11U; /* time slots → which LED */
static constexpr uint8_t REG_MULTI_LED_CTRL2 = 0x12U;
static constexpr uint8_t REG_TEMP_INTR = 0x1FU;
static constexpr uint8_t REG_TEMP_FRAC = 0x20U;
static constexpr uint8_t REG_DIE_TEMP = 0x21U;
static constexpr uint8_t REG_PROX_INT_THRESH = 0x30U;
static constexpr uint8_t REG_REV_ID = 0xFEU; /* Part revision */
static constexpr uint8_t REG_PART_ID = 0xFFU; /* Expected 0x15 for MAX30102 */

int Max30102Driver::init()
{
	/* Future: i2c_write_read() to REG_PART_ID; expect 0x15, then program MODE/FIFO/LEDs. */
	(void)kMax30102I2cAddr7;
	(void)REG_PART_ID;
	(void)REG_REV_ID;
	(void)REG_FIFO_DATA;
	(void)REG_MODE_CONFIG;
	(void)REG_SPO2_CONFIG;
	(void)REG_FIFO_CONFIG;
	(void)REG_LED1_PA;
	(void)REG_LED2_PA;
	(void)REG_MULTI_LED_CTRL1;
	(void)REG_MULTI_LED_CTRL2;
	(void)REG_INTR_ENABLE_1;
	(void)REG_INTR_STATUS_1;
	(void)REG_INTR_STATUS_2;
	(void)REG_INTR_ENABLE_2;
	(void)REG_FIFO_WR_PTR;
	(void)REG_FIFO_RD_PTR;
	(void)REG_OVF_COUNTER;
	(void)REG_PILOT_PA;
	(void)REG_TEMP_INTR;
	(void)REG_TEMP_FRAC;
	(void)REG_DIE_TEMP;
	(void)REG_PROX_INT_THRESH;

	LOG_DBG("MAX30102 skeleton init (no I2C); Part ID reg 0x%02X", REG_PART_ID);
	initialized_ = true;
	return 0;
}

int Max30102Driver::read_sample(uint32_t& out_sample)
{
	if (!initialized_) {
		return -EINVAL;
	}
	(void)out_sample;
	/* No FIFO drain until I2C + interrupt path exists. */
	return -ENOTSUP;
}
