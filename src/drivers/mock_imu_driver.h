#pragma once

#include "imu_driver.h"

#include <cstdint>

/** Replays data/imu_sample.csv (ax,ay,az rows). */
class MockImuDriver final : public IImuDriver {
public:
	MockImuDriver();
	int init() override;
	int read_sample(ImuSample& out) override;
	uint32_t sample_rate_hz() const override { return rate_hz_; }
	void set_sample_rate_hz(uint32_t hz) { rate_hz_ = hz; }

private:
	uint32_t rate_hz_;
	unsigned index_{0};
	bool initialized_{false};
};
