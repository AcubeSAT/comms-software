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
private:
    constexpr static uint16_t DelayMs = 10;
    constexpr static uint16_t TaskStackDepth = 2000;
    constexpr static uint8_t LoggerPrecision = 2;
    constexpr static uint16_t MaxPacketLength = 64;
    constexpr static uint16_t PllFrequency09 = 0x0927;
    constexpr static uint8_t PllChannelNumber09 = 0x0062;

    QueueHandle_t packetQueue;
    AT86RF215::Error error;
    StackType_t taskStack[TaskStackDepth];

public:

    void execute();

    TransceiverTask() : Task("External Temperature Sensors") {
        packetQueue = xQueueCreate(MaxPacketLength, sizeof(uint16_t));
    }

    void addToQueue(const etl::array<uint8_t, MaxPacketLength> &message) {
        xQueueSendToBack(packetQueue, &message, 0);
    }
    AT86RF215::AT86RF215Configuration configFrequency;

    void createRandomPacket(etl::array<uint8_t, MaxPacketLength> &packet, uint16_t length);

    void calculateConfigurationValues(uint32_t frequency);
    void setConfiguration(uint16_t pllFrequency09, uint8_t pllChannelNumber09);
    static AT86RF215::AT86RF215 transceiver;
    void createTask() {
        xTaskCreateStatic(vClassTask < TransceiverTask > , this->TaskName,
                          TransceiverTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));

    }


};

inline std::optional<TransceiverTask> transceiverTask;