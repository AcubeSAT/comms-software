#pragma once

#include <optional>
#include "Task.hpp"
#include "main.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
extern SPI_HandleTypeDef hspi1;

class TransceiverTask : public Task {
private:
    const static inline uint16_t DelayMs = 1000;
    const static inline uint16_t TaskStackDepth = 2000;
    const static inline uint8_t LoggerPrecision = 2;
    AT86RF215::Error error;

    StackType_t taskStack[TaskStackDepth];
public:

    void execute();
    TransceiverTask() : Task("External Temperature Sensors") {}
    AT86RF215::AT86RF215 transceiver = AT86RF215::AT86RF215(&hspi1, AT86RF215::AT86RF215Configuration());

    void createTask(){
        xTaskCreateStatic(vClassTask<TransceiverTask>, this->TaskName,
                          TransceiverTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));

    }
};

inline std::optional<TransceiverTask> transceiverTask;
