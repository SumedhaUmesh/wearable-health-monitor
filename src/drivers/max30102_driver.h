#pragma once

#include "ppg_driver.h"

/**
 * MAX30102 pulse oximeter / PPG (I2C). Skeleton only: register map from datasheet,
 * no bus traffic yet — keeps QEMU bring-up unblocked while documenting intent.
 */
class Max30102Driver final : public IPpgDriver {
public:
	explicit Max30102Driver() = default;
	int init() override;
	int read_sample(uint32_t& out_sample) override;
	uint32_t sample_rate_hz() const override { return 100U; }

private:
	bool initialized_{false};
};
