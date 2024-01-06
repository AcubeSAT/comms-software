#pragma once

#include <optional>
#include "Task.hpp"
#include "main.h"

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

    const std::string errorStrings[7] = {"NoErrors",
                                         "Timeout",
                                         "InvalidEEPROM",
                                         "TemperatureHigh",
                                         "TemperatureLow",
                                         "NoDataReady",
                                         "InvalidCalibrationOffset"};
    const std::string sensorNames[3] = { "PCB MCU",
                                         "PCB UHF PA",
                                         "PCB SBAND PA"};
    Parameter<uint16_t> PlatformParameters[3] = { PlatformParameters::commsPCBTemperatureMCU,
                                                  PlatformParameters::commsPCBTemperatureUHF,
                                                  PlatformParameters::commsPCBTemperatureSBAND };

    StackType_t taskStack[TaskStackDepth];
};

inline std::optional<TemperatureSensorsTask> temperatureSensorsTask;
