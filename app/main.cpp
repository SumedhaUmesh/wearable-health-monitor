/*
 * Wearable Health Monitor — Phase 2–3: sensors + queues + on-device algorithm hooks.
 */

#include "algorithms/activity.h"
#include "algorithms/heart_rate.h"
#include "drivers/imu_driver.h"
#include "drivers/mock_imu_driver.h"
#include "drivers/mock_ppg_driver.h"
#include "services/sensor_manager.h"

#include <cmath>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

int main(void)
{
	MockPpgDriver ppg;
	MockImuDriver imu;
	SensorManager mgr;
	whm::HeartRateEstimator hr_est;
	whm::ActivityClassifier act_clf;

	uint32_t ppg_storage[128];
	ImuSample imu_storage[128];
	struct k_msgq ppg_q;
	struct k_msgq imu_q;

	k_msgq_init(&ppg_q, reinterpret_cast<char *>(ppg_storage), sizeof(uint32_t),
		    ARRAY_SIZE(ppg_storage));
	k_msgq_init(&imu_q, reinterpret_cast<char *>(imu_storage), sizeof(ImuSample),
		    ARRAY_SIZE(imu_storage));

	if (ppg.init() != 0 || imu.init() != 0) {
		LOG_ERR("Driver init failed");
		return 0;
	}

	if (mgr.init(ppg, imu, ppg_q, imu_q) != 0) {
		LOG_ERR("Sensor manager init failed");
		return 0;
	}

	if (mgr.start() != 0) {
		LOG_ERR("Sensor manager start failed");
		return 0;
	}

	hr_est.reset();
	act_clf.reset();

	LOG_INF("Sensor pipeline running (mock PPG + mock IMU + HR/activity hooks).");

	for (;;) {
		unsigned ppg_drained = 0U;
		uint32_t last_ppg = 0U;
		while (k_msgq_get(&ppg_q, &last_ppg, K_NO_WAIT) == 0) {
			++ppg_drained;
			float bpm = 0.0F;
			if (hr_est.push_sample(static_cast<double>(last_ppg), bpm)) {
				LOG_INF("BPM estimate: %.1f", static_cast<double>(bpm));
			}
		}

		unsigned imu_drained = 0U;
		ImuSample last_imu{};
		while (k_msgq_get(&imu_q, &last_imu, K_NO_WAIT) == 0) {
			++imu_drained;
			const float gx = static_cast<float>(last_imu.x);
			const float gy = static_cast<float>(last_imu.y);
			const float gz = static_cast<float>(last_imu.z);
			const float mag = std::sqrt(gx * gx + gy * gy + gz * gz);
			whm::ActivityLabel lab = whm::ActivityLabel::Still;
			if (act_clf.push_magnitude(mag, lab)) {
				LOG_INF("Activity class: %u (steps=%u)", static_cast<unsigned>(lab),
					act_clf.steps());
			}
		}

		if (imu_drained > 0U) {
			LOG_INF("queues: PPG drained=%u last=%u; IMU drained=%u last=(%d,%d,%d)",
				ppg_drained, last_ppg, imu_drained, last_imu.x, last_imu.y,
				last_imu.z);
		} else {
			LOG_INF("queues: PPG drained=%u last=%u; IMU drained=0", ppg_drained,
				last_ppg);
		}

		k_sleep(K_SECONDS(2));
	}

	return 0;
}
