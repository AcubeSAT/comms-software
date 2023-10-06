#pragma once

#include "Task.hpp"
#include "main.h"
#include "etl/optional.h"
#include "stm32h7xx_hal_conf.h"

extern IWDG_HandleTypeDef hiwdg1;

class WatchdogTask : public Task {
public:
    void execute();

    WatchdogTask() : Task("Watchdog") {}

    void createTask() {
        xTaskCreateStatic(vClassTask < WatchdogTask > , this->TaskName, WatchdogTask::TaskStackDepth, this,
                          tskIDLE_PRIORITY-1, this->taskStack, &(this->taskBuffer));
    }

private:
    /**
     * @brief Calculates and stores the window time in milliseconds.
     *
     * This constant represents the duration during which the watchdog counter should
     * be refreshed to prevent a reset. Since the maximum reload time is 16000ms, the RefreshTime is set to 12000ms.
     */
    const uint16_t RefreshTime = 12000;

    static constexpr uint16_t TaskStackDepth = 1000;

    StackType_t taskStack[TaskStackDepth];
};

inline etl::optional<WatchdogTask> watchdogTask;