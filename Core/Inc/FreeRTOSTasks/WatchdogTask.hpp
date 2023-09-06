#pragma once

#include "Task.hpp"
#include "main.h"
#include "etl/optional.h"
#include "stm32h7xx_hal_conf.h"

extern IWDG_HandleTypeDef hiwdg1;

class WatchdogTask : public Task {
public:
    void execute();

    WatchdogTask() : Task("Watchdog"),
                     WindowValue(hiwdg1.Instance->WINR),
                     ClockFrequency{LSI_VALUE} {}

    void createTask() {
        xTaskCreateStatic(vClassTask < WatchdogTask > , this->TaskName, WatchdogTask::TaskStackDepth, this,
                          tskIDLE_PRIORITY, this->taskStack, &(this->taskBuffer));
    }

private:
    /**
    * @brief Counter clock prescaler value.
    * The `hiwdg1.Instance->PR` holds an index representing the IWDG prescaler value. The actual value is obtained
    * by raising 2 to the power of (index + 2). Left-shifting 2 by (n-1) gives 2^n so the actual exponential part
    * is hiwdg1.Instance->PR + 1
    */
    const uint16_t CounterClockPrescaler = 2 << (hiwdg1.Instance->PR + 1);

    /**
    * @brief Window value for IWDG.
    * This value defines the window in which the counter can be refreshed.
    */
    const uint16_t WindowValue = 0;

    /**
    * @brief Clock frequency for the IWDG.
    * Represents the base frequency before applying any prescaler.
    */
    const uint16_t ClockFrequency = 0;

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
};

inline etl::optional<WatchdogTask> watchdogTask;

