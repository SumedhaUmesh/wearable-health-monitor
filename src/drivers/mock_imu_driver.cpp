#include "mock_imu_driver.h"

#include <errno.h>

/* Rows match data/imu_sample.csv after header. */
static constexpr ImuSample kImuSamples[] = {
	{-120, 480, 16320, 0}, {-118, 482, 16325, 0}, {-115, 485, 16318, 0}, {-122, 478, 16330, 0},
	{-119, 481, 16322, 0}, {-117, 483, 16328, 0}, {-121, 479, 16315, 0}, {-116, 486, 16332, 0},
};

MockImuDriver::MockImuDriver() : rate_hz_{50U} {}

int MockImuDriver::init()
{
	index_ = 0;
	initialized_ = true;
	return 0;
}

int MockImuDriver::read_sample(ImuSample& out)
{
	if (!initialized_) {
		return -EINVAL;
	}
	out = kImuSamples[index_];
	const unsigned n = sizeof(kImuSamples) / sizeof(kImuSamples[0]);
	index_ = (index_ + 1U) % n;
	return 0;
}
