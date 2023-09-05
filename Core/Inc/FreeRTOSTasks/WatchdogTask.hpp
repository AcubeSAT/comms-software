#pragma once

#include <optional>
#include "Task.hpp"
#include "main.h"

extern IWDG_HandleTypeDef hiwdg1;

class WatchdogTask : public Task {
private:

    /**
    * @brief Counter clock prescaler value.
    * Represents the division factor applied to the IWDG clock.
    */
    const uint16_t CounterClockPrescaler = 128;

    /**
    * @brief Window value for IWDG.
    * This value defines the window in which the counter can be refreshed.
    */
    const uint16_t WindowValue = 4095;

    /**
    * @brief Down-counter reload value.
    * The counter counts down from this value. When it reaches zero, a reset is triggered.
    */
    const uint16_t DownCounterReloadValue = 4095;

    /**
    * @brief Clock frequency for the IWDG.
    * Represents the base frequency before applying any prescaler.
    */
    const uint16_t ClockFrequency = 32000;

    /**
     * @brief Calculates and stores the window time in milliseconds.
     *
     * This constant represents the duration during which the watchdog counter should
     * be refreshed to prevent a reset. It's calculated using the window value, counter
     * clock prescaler, and the clock frequency.
     */
    const uint16_t WindowTime = 1000 * (WindowValue * CounterClockPrescaler) / ClockFrequency;

    const static inline uint16_t TaskStackDepth = 1000;

    StackType_t taskStack[TaskStackDepth];

public:
    void execute();

    WatchdogTask() : Task("Watchdog") {}

    void createTask() {
        xTaskCreateStatic(vClassTask < WatchdogTask > , this->TaskName, WatchdogTask::TaskStackDepth, this,
                          tskIDLE_PRIORITY, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<WatchdogTask> watchdogTask;

