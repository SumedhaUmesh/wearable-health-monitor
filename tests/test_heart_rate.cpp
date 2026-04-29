#include <cmath>
#include <gtest/gtest.h>

#include "algorithms/heart_rate.h"

TEST(HeartRate, RoughBpmFromSinePpg)
{
	static constexpr double kPi = 3.14159265358979323846;
	whm::HeartRateEstimator hr;
	hr.reset();
	const double fs = 25.0;
	const double f_hz = 1.2;
	const double amp = 500.0;
	const double off = 2000.0;
	bool got = false;
	float bpm = 0.0F;
	for (int n = 0; n < 600; ++n) {
		const double t = static_cast<double>(n) / fs;
		const double x = off + amp * std::sin(2.0 * kPi * f_hz * t);
		if (hr.push_sample(x, bpm)) {
			got = true;
		}
	}
	ASSERT_TRUE(got);
	EXPECT_NEAR(static_cast<double>(bpm), 72.0, 6.0);
}
