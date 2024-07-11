#pragma once

#include "Task.hpp"
#include "main.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "queue.h"
#include <etl/optional.h>

extern SPI_HandleTypeDef hspi1;

class IQTransmissionTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];

    AT86RF215::AT86RF215Configuration configFrequency;

    constexpr static uint16_t DelayMs = 10;
    constexpr static uint32_t FrequencyUHF = 436500;

    QueueHandle_t packetQueue;
    AT86RF215::Error error;
public:
    void execute();

    static inline int interruptCount = 0;

    static AT86RF215::AT86RF215 transceiver;

    IQTransmissionTask() : Task("IQ Transmission") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<IQTransmissionTask>, this->TaskName,
                          IQTransmissionTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }

    /*
     * This function calculates the PllChannelFrequency value using the formula given in the datasheet
     * for Fine Resolution Channel Scheme CNM.CM=1 (section 6.3.2)
     */
    uint16_t calculatePllChannelFrequency24(uint32_t frequency);

    /*
     * This function calculates the PllChannelNumber value using the formula given in the datasheet
     * for Fine Resolution Channel Scheme CNM.CM=1 (section 6.3.2)
     */
    uint8_t calculatePllChannelNumber24(uint32_t frequency);

    /*
     * This function allows to easily configure the most important settings
     */
    void setConfiguration(uint16_t pllFrequency24, uint8_t pllChannelNumber24);

    constexpr static uint32_t FrequencyS = 2425000;
};

inline std::optional<IQTransmissionTask> iqTransmissionTask;