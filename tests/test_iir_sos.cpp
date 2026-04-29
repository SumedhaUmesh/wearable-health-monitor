#include <gtest/gtest.h>

#include "algorithms/heart_rate.h"

/* Impulse response first 30 samples (scipy.signal.sosfilt on same SOS, impulse). */
static constexpr double kGolden30[30] = {
	0.117351037, 0.309376045, 0.267459079, 0.033631428, -0.134616763, -0.182853840,
	-0.156900584, -0.112814577, -0.080010414, -0.062817049, -0.054569242, -0.048440686,
	-0.041285802, -0.032972422, -0.024507864, -0.016760119, -0.010079200, -0.004437903,
	0.000321521,  0.004336038,  0.007684401,  0.010408584,  0.012543197,  0.014130953,
	0.015224216,  0.015880256,  0.016156371,  0.016107170,  0.015783741,  0.015233652,
};

TEST(IirSos, ImpulseMatchesScipyGolden)
{
	whm::SosBandpass25Hz f;
	f.reset();
	EXPECT_NEAR(f.process(1.0), kGolden30[0], 1e-6);
	for (int i = 1; i < 30; ++i) {
		EXPECT_NEAR(f.process(0.0), kGolden30[i], 1e-6);
	}
}
