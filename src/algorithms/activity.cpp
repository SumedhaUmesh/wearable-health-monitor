#include "activity.h"

namespace whm {

ActivityClassifier::ActivityClassifier(const ActivityConfig& cfg) : cfg_(cfg) {}

void ActivityClassifier::reset()
{
	filled_ = 0U;
	steps_ = 0U;
	prev_centered_ = 0.0F;
	have_prev_ = false;
	for (unsigned i = 0U; i < kMaxWindow; ++i) {
		buf_[i] = 0.0F;
	}
}

unsigned ActivityClassifier::window_samples() const
{
	const unsigned w = cfg_.sample_rate_hz * cfg_.window_sec;
	return (w < kMaxWindow) ? w : kMaxWindow;
}

void ActivityClassifier::count_step_zero_cross(float centered)
{
	static constexpr float kThresh = 200.0F;
	if (!have_prev_) {
		have_prev_ = true;
		prev_centered_ = centered;
		return;
	}
	if (prev_centered_ < -kThresh && centered >= -kThresh) {
		++steps_;
	}
	prev_centered_ = centered;
}

bool ActivityClassifier::push_magnitude(float mag, ActivityLabel& label_out)
{
	const unsigned ws = window_samples();
	if (filled_ < ws) {
		buf_[filled_++] = mag;
		return false;
	}
	for (unsigned i = 1U; i < ws; ++i) {
		buf_[i - 1U] = buf_[i];
	}
	buf_[ws - 1U] = mag;

	double sum = 0.0;
	for (unsigned i = 0U; i < ws; ++i) {
		sum += static_cast<double>(buf_[i]);
	}
	const double mean = sum / static_cast<double>(ws);
	double var = 0.0;
	for (unsigned i = 0U; i < ws; ++i) {
		const double d = static_cast<double>(buf_[i]) - mean;
		var += d * d;
	}
	var /= static_cast<double>(ws);

	const float vf = static_cast<float>(var);
	if (vf < cfg_.var_still_max) {
		label_out = ActivityLabel::Still;
	} else if (vf < cfg_.var_walk_max) {
		label_out = ActivityLabel::Walking;
	} else {
		label_out = ActivityLabel::Running;
	}

	const float centered = mag - static_cast<float>(mean);
	count_step_zero_cross(centered);
	return true;
}

} /* namespace whm */
