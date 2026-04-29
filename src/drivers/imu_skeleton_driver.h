#pragma once

#include "imu_driver.h"

/**
 * Placeholder 3-axis MEMS on I2C (no specific vendor in Phase 2).
 * Register names mirror common WHO_AM_I / CTRL / DATA patterns for later wiring.
 */
class ImuSkeletonDriver final : public IImuDriver {
public:
	int init() override;
	int read_sample(ImuSample& out) override;
	uint32_t sample_rate_hz() const override { return 52U; }

private:
	bool initialized_{false};
};
