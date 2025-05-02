#pragma once

#include <optional>
#include "Task.hpp"
#include "etl/string.h"

class TransceiverBasebandCoreTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 3000;

    StackType_t taskStack[TaskStackDepth];
public:
    void execute();

    bool txRx = true; // true for tx, false for rx

    uint8_t dummyPacket10[10];
    uint8_t dummyPacket50[50];
    uint8_t dummyPacket256[255];
    uint8_t cyclicalCounter = 0;

    uint8_t receivedPacket[500];
    uint16_t receivedPacketLength = 0;
    etl::string<500> receivedPacketString;

    TransceiverBasebandCoreTask() : Task("TransceiverBasebandCore") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<TransceiverBasebandCoreTask>, this->TaskName,
                          TransceiverBasebandCoreTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<TransceiverBasebandCoreTask> transceiverBasebandCore;

