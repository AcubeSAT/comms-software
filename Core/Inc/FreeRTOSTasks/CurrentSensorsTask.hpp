#pragma once

#include <etl/optional.h>
#include "Task.hpp"

extern I2C_HandleTypeDef hi2c2;

class CurrentSensorsTask : public Task {
public:

    void execute();

    CurrentSensorsTask() : Task("Current Sensors") {}

    void createTask() {
        xTaskCreateStatic(vClassTask < CurrentSensorsTask > , this->TaskName,
                          CurrentSensorsTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }

private:
    static constexpr uint16_t DelayMs = 1000;
    static constexpr uint16_t TaskStackDepth = 2000;
    static constexpr uint8_t Precision = 3;

    StackType_t taskStack[TaskStackDepth];
};

inline etl::optional<CurrentSensorsTask> currentSensorsTask;
