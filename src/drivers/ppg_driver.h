#pragma once

#include <cstdint>

/** PPG front-end: real part (MAX30102) or replay (mock) behind the same API. */
class IPpgDriver {
public:
	virtual ~IPpgDriver() = default;
	virtual int init() = 0;
	/** 0 on success; negative errno (e.g. -EAGAIN) if no sample yet. */
	virtual int read_sample(uint32_t& out_sample) = 0;
	virtual uint32_t sample_rate_hz() const = 0;
};
