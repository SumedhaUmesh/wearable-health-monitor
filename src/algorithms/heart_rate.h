#pragma once

namespace whm {

/** Second-order sections bandpass (fixed coeffs in heart_rate_coeffs.h). */
class SosBandpass25Hz {
public:
	SosBandpass25Hz();
	void reset();
	double process(double x);

private:
	static constexpr unsigned kSections = 2U;
	double v1_[kSections]{};
	double v2_[kSections]{};
};

/**
 * PPG → bandpass → causal peak picking (refractory) → BPM over a sliding window.
 * Sample clock is fixed at 25 Hz to match mock PPG and filter design.
 */
class HeartRateEstimator {
public:
	void reset();
	/** Raw PPG counts or normalized amplitude. Returns true when `bpm` was refreshed. */
	bool push_sample(double raw_ppg, float& bpm);

private:
	static constexpr float kSampleRateHz = 25.0F;
	static constexpr float kRefractorySec = 0.30F;
	static constexpr float kBpmWindowSec = 8.0F;
	static constexpr unsigned kPeakHistoryMax = 64U;

	SosBandpass25Hz filter_{};
	double prev_f_{0.0};
	double prev2_f_{0.0};
	unsigned since_last_peak_{1000U};
	unsigned sample_index_{0U};
	unsigned peak_indices_[kPeakHistoryMax]{};
	unsigned peak_count_{0U};

	void record_peak();
	bool compute_bpm(float& out);
};

} /* namespace whm */
