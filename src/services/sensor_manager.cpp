#include "sensor_manager.h"

#include "drivers/imu_driver.h"
#include "drivers/ppg_driver.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

LOG_MODULE_REGISTER(sensor_mgr, LOG_LEVEL_INF);

void sensor_manager_ppg_work(struct k_work *work)
{
	struct k_work_delayable *d = k_work_delayable_from_work(work);
	SensorManager *self = CONTAINER_OF(d, SensorManager, ppg_dwork_);
	self->on_ppg_tick();
}

void sensor_manager_imu_work(struct k_work *work)
{
	struct k_work_delayable *d = k_work_delayable_from_work(work);
	SensorManager *self = CONTAINER_OF(d, SensorManager, imu_dwork_);
	self->on_imu_tick();
}

int SensorManager::init(IPpgDriver& ppg, IImuDriver& imu, struct k_msgq& ppg_q, struct k_msgq& imu_q)
{
	ppg_ = &ppg;
	imu_ = &imu;
	ppg_q_ = &ppg_q;
	imu_q_ = &imu_q;

	const uint32_t ppg_hz = ppg_->sample_rate_hz();
	const uint32_t imu_hz = imu_->sample_rate_hz();
	if (ppg_hz == 0U || imu_hz == 0U) {
		return -EINVAL;
	}
	ppg_period_ms_ = (1000U + ppg_hz / 2U) / ppg_hz;
	imu_period_ms_ = (1000U + imu_hz / 2U) / imu_hz;
	if (ppg_period_ms_ == 0U) {
		ppg_period_ms_ = 1U;
	}
	if (imu_period_ms_ == 0U) {
		imu_period_ms_ = 1U;
	}

	k_work_init_delayable(&ppg_dwork_, sensor_manager_ppg_work);
	k_work_init_delayable(&imu_dwork_, sensor_manager_imu_work);
	return 0;
}

int SensorManager::start()
{
	if (!ppg_ || !imu_ || !ppg_q_ || !imu_q_) {
		return -EINVAL;
	}
	if (started_) {
		return 0;
	}
	(void)k_work_schedule(&ppg_dwork_, K_NO_WAIT);
	(void)k_work_schedule(&imu_dwork_, K_NO_WAIT);
	started_ = true;
	return 0;
}

void SensorManager::stop()
{
	if (!started_) {
		return;
	}
	(void)k_work_cancel_delayable(&ppg_dwork_);
	(void)k_work_cancel_delayable(&imu_dwork_);
	started_ = false;
}

void SensorManager::on_ppg_tick()
{
	uint32_t sample = 0U;
	const int err = ppg_->read_sample(sample);
	if (err == 0) {
		if (k_msgq_put(ppg_q_, &sample, K_NO_WAIT) != 0) {
			LOG_WRN("PPG queue full; dropping sample");
		}
	}
	(void)k_work_reschedule(&ppg_dwork_, K_MSEC(ppg_period_ms_));
}

void SensorManager::on_imu_tick()
{
	ImuSample sample{};
	const int err = imu_->read_sample(sample);
	if (err == 0) {
		if (k_msgq_put(imu_q_, &sample, K_NO_WAIT) != 0) {
			LOG_WRN("IMU queue full; dropping sample");
		}
	}
	(void)k_work_reschedule(&imu_dwork_, K_MSEC(imu_period_ms_));
}
