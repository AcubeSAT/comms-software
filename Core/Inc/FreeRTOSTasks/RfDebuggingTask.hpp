#pragma once

#include <optional>
#include "Task.hpp"
#include "etl/string.h"

enum Process {
    INTERRUPT_TEST,    // toggle between TRXOFF and TXPREP to test if the tranceiver interrupt is received properly
    BASEBAND_TX_UHF,       // use transceiver's sub GHz baseband core and transmit dummy packets in UHF, 436.5MHz with BFSK
    BASEBAND_RX_UHF,       // use transceiver's sub GHz baseband core and receive dummy packets in UHF, 436.5MHz with BFSK
    POWER_MEASUREMENT_UHF,    // measure channel power in dBm
    POWER_MEASUREMENT_SBAND,
    LO_CARRIER_UHF,    // sent a non modulated carrier
    LO_CARRIER_SBAND,
    IQ_LOOPBACK_MODE   // sets the transceiver's I/Q interface to loopback mode, meaning any packets sent to
                       // TXD are returned back to RXD09/24 (with a delay of up to 18-bit periods). No other action is
                       // performed. Useful for FPGA tests.
};

class RfDebuggingTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 5000;

    StackType_t taskStack[TaskStackDepth];
public:
    [[noreturn]] void execute();

    Process process = INTERRUPT_TEST;
    static constexpr uint32_t timeoutDelayMs = 10000; // How long the transceiver will wait for baseband packet reception until it times out

    uint8_t dummyPacket10[10];
    uint8_t dummyPacket50[50];
    uint8_t dummyPacket256[256];
    uint8_t cyclicalCounter = 0;

    uint8_t receivedPacket[500];
    uint16_t receivedPacketLength = 0;
    etl::string<500> receivedPacketString;

    RfDebuggingTask() : Task("RfDebuggingTask") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<RfDebuggingTask>, this->TaskName,
                          RfDebuggingTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<RfDebuggingTask> rfDebuggingTask;

