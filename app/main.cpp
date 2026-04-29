/*
 * Wearable Health Monitor — Phase 1 bring-up.
 * Logs a single "Hello" line via Zephyr logging (visible on QEMU console).
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

int main(void)
{
	LOG_INF("Hello.");
	return 0;
}
