#include "mock_ppg_driver.h"

#include <errno.h>

/* Keep in sync with data/ppg_sample.csv (value column). */
static constexpr uint32_t kPpgSamples[] = {
	512U, 518U, 525U, 530U, 528U, 522U, 515U, 510U, 508U, 512U,
	520U, 530U, 538U, 535U, 528U, 520U, 515U, 518U, 528U, 540U,
	548U, 542U, 530U, 518U, 512U, 515U, 525U, 535U, 532U, 520U,
};

MockPpgDriver::MockPpgDriver() : rate_hz_{25U} {}

int MockPpgDriver::init()
{
	index_ = 0;
	initialized_ = true;
	return 0;
}

int MockPpgDriver::read_sample(uint32_t& out_sample)
{
	if (!initialized_) {
		return -EINVAL;
	}
	out_sample = kPpgSamples[index_];
	const unsigned n = sizeof(kPpgSamples) / sizeof(kPpgSamples[0]);
	index_ = (index_ + 1U) % n;
	return 0;
}
