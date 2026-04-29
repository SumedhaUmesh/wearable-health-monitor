#include "heart_rate.h"

#include "heart_rate_coeffs.h"

#include <cmath>

namespace whm {

SosBandpass25Hz::SosBandpass25Hz() = default;

void SosBandpass25Hz::reset()
{
	for (unsigned s = 0U; s < kSections; ++s) {
		v1_[s] = 0.0;
		v2_[s] = 0.0;
	}
}

double SosBandpass25Hz::process(double x)
{
	double in = x;
	for (unsigned s = 0U; s < kSections; ++s) {
		const double b0 = whm::hr_coeffs::kSos[s][0];
		const double b1 = whm::hr_coeffs::kSos[s][1];
		const double b2 = whm::hr_coeffs::kSos[s][2];
		const double a0 = whm::hr_coeffs::kSos[s][3];
		const double a1 = whm::hr_coeffs::kSos[s][4];
		const double a2 = whm::hr_coeffs::kSos[s][5];
		const double inv_a0 = 1.0 / a0;
		const double bb0 = b0 * inv_a0;
		const double bb1 = b1 * inv_a0;
		const double bb2 = b2 * inv_a0;
		const double aa1 = a1 * inv_a0;
		const double aa2 = a2 * inv_a0;

		const double v0 = in - aa1 * v1_[s] - aa2 * v2_[s];
		const double y = bb0 * v0 + bb1 * v1_[s] + bb2 * v2_[s];
		v2_[s] = v1_[s];
		v1_[s] = v0;
		in = y;
	}
	return in;
}

void HeartRateEstimator::reset()
{
	filter_.reset();
	prev_f_ = 0.0;
	prev2_f_ = 0.0;
	since_last_peak_ = 1000U;
	sample_index_ = 0U;
	peak_count_ = 0U;
}

void HeartRateEstimator::record_peak()
{
	if (peak_count_ < kPeakHistoryMax) {
		peak_indices_[peak_count_++] = sample_index_;
	} else {
		for (unsigned i = 1U; i < kPeakHistoryMax; ++i) {
			peak_indices_[i - 1U] = peak_indices_[i];
		}
		peak_indices_[kPeakHistoryMax - 1U] = sample_index_;
	}
}

bool HeartRateEstimator::compute_bpm(float& out)
{
	const unsigned window_samples =
		static_cast<unsigned>(kBpmWindowSec * kSampleRateHz + 0.5F);
	const unsigned start_cutoff =
		(sample_index_ > window_samples) ? (sample_index_ - window_samples) : 0U;

	unsigned peaks_in_window = 0U;
	unsigned idx_buf[HeartRateEstimator::kPeakHistoryMax]{};
	for (unsigned i = 0U; i < peak_count_; ++i) {
		if (peak_indices_[i] >= start_cutoff) {
			idx_buf[peaks_in_window++] = peak_indices_[i];
		}
	}
	if (peaks_in_window < 2U) {
		return false;
	}

	double sum_interval = 0.0;
	unsigned intervals = 0U;
	for (unsigned i = 1U; i < peaks_in_window; ++i) {
		const unsigned di = idx_buf[i] - idx_buf[i - 1U];
		if (di > 0U) {
			sum_interval += static_cast<double>(di);
			++intervals;
		}
	}
	if (intervals == 0U) {
		return false;
	}
	const double mean_samples = sum_interval / static_cast<double>(intervals);
	const double bpm = 60.0 * static_cast<double>(kSampleRateHz) / mean_samples;
	if (!std::isfinite(bpm) || bpm < 30.0 || bpm > 220.0) {
		return false;
	}
	out = static_cast<float>(bpm);
	return true;
}

bool HeartRateEstimator::push_sample(double raw_ppg, float& bpm)
{
	const double f = filter_.process(raw_ppg);

	const unsigned min_sep =
		static_cast<unsigned>(kRefractorySec * kSampleRateHz + 0.5F);
	if (since_last_peak_ < 1000U) {
		++since_last_peak_;
	}

	/* Causal peak at n-1: previous sample was a local maximum. */
	if (sample_index_ >= 2U) {
		const double mid = prev_f_;
		if (mid > prev2_f_ && mid >= f && mid > 1e-6) {
			if (since_last_peak_ >= min_sep) {
				record_peak();
				since_last_peak_ = 0U;
			}
		}
	}

	prev2_f_ = prev_f_;
	prev_f_ = f;
	++sample_index_;

	return compute_bpm(bpm);
}

} /* namespace whm */
