#pragma once

#include <optional>
#include "Task.hpp"
#include "main.h"

extern I2C_HandleTypeDef hi2c2;

class TemperatureSensorsTask : public Task {
private:
    const static inline uint16_t DelayMs = 1000;
    const static inline uint16_t TaskStackDepth = 2000;
    const static inline uint8_t LoggerPrecision = 2;

    StackType_t taskStack[TaskStackDepth];
public:

    void execute();

    TemperatureSensorsTask() : Task("External Temperature Sensors") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<TemperatureSensorsTask>, this->TaskName,
                          TemperatureSensorsTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<TemperatureSensorsTask> temperatureSensorsTask;
