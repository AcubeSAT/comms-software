#pragma once

#include <optional>
#include "Task.hpp"
#include "main.h"

extern I2C_HandleTypeDef hi2c2;

class CurrentSensorsTask : public Task {
private:
    static constexpr uint16_t DelayMs = 1000;
    static constexpr uint16_t TaskStackDepth = 2000;
    static constexpr uint8_t Precision = 2;

    StackType_t taskStack[TaskStackDepth];

public:

    void execute();

    CurrentSensorsTask() : Task("Current Sensors") {}

    void createTask() {
        xTaskCreateStatic(vClassTask < CurrentSensorsTask > , this->TaskName,
                          CurrentSensorsTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<CurrentSensorsTask> currentSensorsTask;