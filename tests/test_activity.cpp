#include <cmath>
#include <gtest/gtest.h>

#include "algorithms/activity.h"

TEST(Activity, StillVsRunningByVariance)
{
	whm::ActivityClassifier clf;
	clf.reset();
	whm::ActivityLabel lab = whm::ActivityLabel::Walking;
	for (unsigned i = 0U; i < 120U; ++i) {
		(void)clf.push_magnitude(16384.0F, lab);
	}
	bool done = false;
	for (unsigned i = 0U; i < 120U; ++i) {
		if (clf.push_magnitude(16384.0F, lab)) {
			done = true;
			EXPECT_EQ(lab, whm::ActivityLabel::Still);
			break;
		}
	}
	EXPECT_TRUE(done);

	clf.reset();
	done = false;
	for (unsigned i = 0U; i < 240U; ++i) {
		const float mag =
			16384.0F + 2500.0F * std::sin(static_cast<float>(i) * 0.35F);
		if (clf.push_magnitude(mag, lab)) {
			done = true;
			EXPECT_EQ(lab, whm::ActivityLabel::Running);
			break;
		}
	}
	EXPECT_TRUE(done);
}
