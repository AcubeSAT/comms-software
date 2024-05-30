#pragma once

#include "Task.hpp"
#include "main.h"
#include "at86rf215.hpp"
#include "at86rf215customConfig.hpp"
#include "queue.h"
#include <etl/optional.h>

extern SPI_HandleTypeDef hspi1;

class TransceiverTask : public Task {
public:
    void execute();

    constexpr static uint16_t MaxPacketLength = 2000;
    using PacketType = etl::array<uint8_t, MaxPacketLength>;

    TransceiverTask() : Task("Transceiver signal transmission") {}

    /*
     * This function creates random packets until we have full functionality.
     */
    PacketType createRandomPacket(uint16_t length);

    static AT86RF215::AT86RF215 transceiver;

    void createTask() {
        xTaskCreateStatic(vClassTask < TransceiverTask > , this->TaskName,
                          TransceiverTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }

    uint8_t checkTheSPI();

private:

    constexpr static uint16_t DelayMs = 10;
    constexpr static uint16_t TaskStackDepth = 2000;

    QueueHandle_t packetQueue;
    AT86RF215::Error error;
    StackType_t taskStack[TaskStackDepth];

    // Enable/Disable tranceiver functionality (Do not set both transceivers to Rx, since there is only one Rx buffer)
    constexpr static bool enableBBC0 = true;
    constexpr static bool enableBBC1 = true;
    constexpr static bool RxTxUHF = false;     // 0 for Rx, 1 for Tx. Has effect only if BBC0 is enabled
    constexpr static bool RxTxSBAND = false;  // 0 for Rx, 1 for Tx. Has effect only if BBC1 is enabled

};

inline etl::optional<TransceiverTask> transceiverTask;