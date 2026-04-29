#pragma once

#include <zephyr/kernel.h>

class IPpgDriver;
class IImuDriver;

/** Pulls from drivers on separate cadences and enqueues raw samples for later algorithms. */
class SensorManager {
public:
	SensorManager() = default;
	int init(IPpgDriver& ppg, IImuDriver& imu, struct k_msgq& ppg_q, struct k_msgq& imu_q);
	int start();
	void stop();

private:
	friend void sensor_manager_ppg_work(struct k_work *work);
	friend void sensor_manager_imu_work(struct k_work *work);
	void on_ppg_tick();
	void on_imu_tick();

	IPpgDriver *ppg_{nullptr};
	IImuDriver *imu_{nullptr};
	struct k_msgq *ppg_q_{nullptr};
	struct k_msgq *imu_q_{nullptr};
	k_work_delayable ppg_dwork_{};
	k_work_delayable imu_dwork_{};
	uint32_t ppg_period_ms_{40U};
	uint32_t imu_period_ms_{20U};
	bool started_{false};
};
