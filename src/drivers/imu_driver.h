#pragma once

#include <cstdint>

/* k_msgq message size should be a multiple of 4 on ARM. */
struct ImuSample {
	int16_t x;
	int16_t y;
	int16_t z;
	uint16_t _pad;
};

/** Accelerometer path: real I2C part or mock replay. */
class IImuDriver {
public:
	virtual ~IImuDriver() = default;
	virtual int init() = 0;
	virtual int read_sample(ImuSample& out) = 0;
	virtual uint32_t sample_rate_hz() const = 0;
};
