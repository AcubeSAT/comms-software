#pragma once

#include <optional>
#include "Task.hpp"
#include "main.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "queue.h"
#include "semphr.h"

extern SPI_HandleTypeDef hspi1;

class TransceiverTask : public Task {
public:
    void execute();

    constexpr static uint16_t MaxPacketLength = 64;
    using Packet = etl::array<uint8_t, MaxPacketLength>;

    TransceiverTask() : Task("Transceiver signal transmission") {
        packetQueue = xQueueCreate(MaxPacketLength, sizeof(uint16_t));
    }

    AT86RF215::AT86RF215Configuration configFrequency;

    /*
     * This function creates random packets until we have full functionality.
     */
    Packet createRandomPacket(uint16_t length);

    /*
     * This function calculates the PllChannelFrequency value using the formula given in the datasheet
     * for Fine Resolution Channel Scheme CNM.CM=1 (section 6.3.2)
     */
    uint16_t calculatePllChannelFrequency09(uint32_t frequency);

    /*
     * This function calculates the PllChannelNumber value using the formula given in the datasheet
     * for Fine Resolution Channel Scheme CNM.CM=1 (section 6.3.2)
     */
    uint8_t calculatePllChannelNumber09(uint32_t frequency);

    void setConfiguration(uint16_t pllFrequency09, uint8_t pllChannelNumber09);

    static AT86RF215::AT86RF215 transceiver;

    void createTask() {
        xTaskCreateStatic(vClassTask < TransceiverTask > , this->TaskName,
                          TransceiverTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }

private:
    constexpr static uint16_t DelayMs = 10;
    constexpr static uint16_t TaskStackDepth = 2000;
    constexpr static uint32_t FrequencyUHF = 436500;

    QueueHandle_t packetQueue;
    AT86RF215::Error error;
    StackType_t taskStack[TaskStackDepth];

};

inline std::optional<TransceiverTask> transceiverTask;