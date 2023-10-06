#include "WatchdogTask.hpp"

void WatchdogTask::execute() {
    while (true) {
        HAL_IWDG_Refresh(&hiwdg1);
        LOG_DEBUG << "Watchdog refreshed";
        vTaskDelay(pdMS_TO_TICKS(RefreshTime));
    }
}