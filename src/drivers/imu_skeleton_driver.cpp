#include "imu_skeleton_driver.h"

#include <errno.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(imu_skel, LOG_LEVEL_DBG);

/* Example 7-bit address block — replace when you pick a concrete part. */
static constexpr uint8_t kImuI2cAddr7 = 0x19U;

static constexpr uint8_t REG_WHO_AM_I = 0x0FU;
static constexpr uint8_t REG_CTRL1 = 0x20U; /* ODR, enable axes */
static constexpr uint8_t REG_CTRL4 = 0x23U; /* scale, BDU */
static constexpr uint8_t REG_OUT_X_L = 0x28U; /* auto-increment burst from here */

int ImuSkeletonDriver::init()
{
	(void)kImuI2cAddr7;
	(void)REG_WHO_AM_I;
	(void)REG_CTRL1;
	(void)REG_CTRL4;
	(void)REG_OUT_X_L;
	LOG_DBG("IMU skeleton init (no I2C)");
	initialized_ = true;
	return 0;
}

int ImuSkeletonDriver::read_sample(ImuSample& out)
{
	if (!initialized_) {
		return -EINVAL;
	}
	(void)out;
	return -ENOTSUP;
}
