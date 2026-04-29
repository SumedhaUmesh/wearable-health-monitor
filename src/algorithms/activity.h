#pragma once

#include <cstdint>

namespace whm {

enum class ActivityLabel : std::uint8_t { Still = 0U, Walking = 1U, Running = 2U };

struct ActivityConfig {
	/** Variance of |a| over the window — tuned for mock / WISDM-scale magnitudes. */
	float var_still_max = 800.0F;
	float var_walk_max = 8000.0F;
	unsigned sample_rate_hz = 50U;
	unsigned window_sec = 2U;
};

class ActivityClassifier {
public:
	explicit ActivityClassifier(const ActivityConfig& cfg = {});
	void reset();
	/** Feed one magnitude sample (e.g. sqrt(x²+y²+z²)); true when a window completed. */
	bool push_magnitude(float mag, ActivityLabel& label_out);
	unsigned steps() const { return steps_; }

private:
	ActivityConfig cfg_{};
	static constexpr unsigned kMaxWindow = 256U;
	float buf_[kMaxWindow]{};
	unsigned filled_{0U};
	unsigned steps_{0U};
	float prev_centered_{0.0F};
	bool have_prev_{false};

	unsigned window_samples() const;
	void count_step_zero_cross(float centered);
};

} /* namespace whm */
