#pragma once

#include "Task.hpp"
#include "DummyTask.h"
#include "TemperatureSensorsTask.hpp"
#include "TransceiverTask.hpp"
#include "WatchdogTask.hpp"
#include "UARTGatekeeperTask.hpp"
#include "MCUTemperatureTask.hpp"
#include "CurrentSensorsTask.hpp"
#include "TimeKeepingTask.hpp"

class InitializationTask: public Task {
public:
    const static inline uint16_t TaskStackDepth = 8300;

    StackType_t taskStack[TaskStackDepth];

    InitializationTask() : Task("InitializationTask") {}

    void execute();

    void createTask() {
        xTaskCreateStatic(vClassTask<InitializationTask>, this->TaskName, InitializationTask::TaskStackDepth,
                          this, configMAX_PRIORITIES - 1, this->taskStack,
                          &(this->taskBuffer));
    }
};

inline std::optional<InitializationTask> initializationTask;
