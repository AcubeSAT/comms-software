#pragma once

#include <optional>
#include "Task.hpp"
#include "main.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "queue.h"

extern SPI_HandleTypeDef hspi1;

class TransceiverTask : public Task {
private:
    constexpr static uint16_t DelayMs = 1000;
    constexpr static uint16_t TaskStackDepth = 2000;
    constexpr static uint8_t LoggerPrecision = 2;
    constexpr static uint16_t MaxPacketLength = 64;

//    uint8_t packet[PacketLength] = {0, 1, 2, 3, 43, 56, 2, 78, 12, 34, 82, 39};
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

    void createRandomPacket(etl::array<uint8_t, MaxPacketLength> &packet, uint16_t length);

    AT86RF215::AT86RF215 transceiver = AT86RF215::AT86RF215(&hspi1, AT86RF215::AT86RF215Configuration());

    void createTask(){
        xTaskCreateStatic(vClassTask<TransceiverTask>, this->TaskName,
                          TransceiverTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));

    }
};

inline std::optional<TransceiverTask> transceiverTask;
