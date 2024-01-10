#pragma once

#include <optional>
#include "Task.hpp"
#include "main.h"
#include "TMP117.hpp"

extern I2C_HandleTypeDef hi2c2;

class TemperatureSensorsTask : public Task {
public:

    void execute();

    TemperatureSensorsTask() : Task("External Temperature Sensors") {}

    void createTask() {
        xTaskCreateStatic(vClassTask < TemperatureSensorsTask > , this->TaskName,
                          TemperatureSensorsTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }

private:
    const static inline uint16_t DelayMs = 1000;
    const static inline uint16_t TaskStackDepth = 2000;
    const static inline uint8_t LoggerPrecision = 2;
    StackType_t taskStack[TaskStackDepth];

    const std::string errorStrings[7] = {"NoErrors",
                                         "Timeout",
                                         "InvalidEEPROM",
                                         "TemperatureHigh",
                                         "TemperatureLow",
                                         "NoDataReady",
                                         "InvalidCalibrationOffset"};

    // This struct holds settings for the sensors. For polling, continuous mode is used
    // (the sensors periodically produce new values). Alert settings are irrelevant for now,
    // since the alert pin was not used.
    TMP117::Config config = {
            TMP117::Continuous,
            static_cast<uint8_t>(7),
            TMP117::Samples8,
            true,
            false,
            true
    };

    struct sensor {
        TMP117::TMP117 sensorObject;
        Parameter<uint16_t>& platformParameterReference;
        const char* sensorName;
    };

};

inline std::optional<TemperatureSensorsTask> temperatureSensorsTask;
