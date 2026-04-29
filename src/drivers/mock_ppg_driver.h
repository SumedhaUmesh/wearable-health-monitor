#pragma once

#include "ppg_driver.h"

#include <cstdint>

/**
 * Replays a fixed PPG waveform (same numeric sequence as data/ppg_sample.csv).
 * Each read_sample() advances one tick; caller pacing sets effective rate.
 */
class MockPpgDriver final : public IPpgDriver {
public:
	MockPpgDriver();
	int init() override;
	int read_sample(uint32_t& out_sample) override;
	uint32_t sample_rate_hz() const override { return rate_hz_; }
	void set_sample_rate_hz(uint32_t hz) { rate_hz_ = hz; }

private:
	uint32_t rate_hz_;
	unsigned index_{0};
	bool initialized_{false};
};
