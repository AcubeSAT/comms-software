#pragma once

#include "Task.hpp"
#include "main.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "queue.h"
#include <etl/optional.h>

extern SPI_HandleTypeDef hspi1; // transceiver's spi handle

class TransceiverTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];

    /**
     * FPGA_PROCESSING_SBAND_TX: FPGA performs the baseband processing and sends the packets using the LVDS interface,
     *                           using the embedded control feature. Only the transceiver radio and SBAND RF frontend
     *                           are powered on. Central frequency of transmission is at 2.425 GHz.
     * FPGA_PROCESSING_LOOPBACK: FPGA performs the baseband processing, but no transmission or reception takes place. Instead,
     *                           the I/Q data sent to the transceiver passes through it's digital frontends and then is looped back
     *                           to the FPGA. This mode exists for diagnostic purposes only. The data is looped back both
     *                           in RXDP09/RXDN09 and RXDP24/RXDN24 differential pins.
     *
     * SLEEP: FPGA, transceiver and external RF frontend are turned off, for power consumption. This mode could be viable
     *        when the sc is not communicating with the gs, and only needs to transmit a beacon message every few seconds.
     *
     */
    enum OperationalMode {
        TRANSCEIVER_PROCESSING_UHF_TXRX,
        FPGA_PROCESSING_UHF_TXRX,
        FPGA_PROCESSING_SBAND_TX,
        FPGA_PROCESSING_LOOPBACK,
        SLEEP
    };

    OperationalMode opMode;
    AT86RF215::Error transceiverError;

    // variables for fpga processing operations
    uint16_t iqSyncCount = 0;

    // variables for transceiver processing operations
    bool rxTx = false;  // false == tx
    uint16_t txPacketLength = 0;
    constexpr static uint16_t txMaxPacketLength = 250;
    etl::array<uint8_t , txMaxPacketLength> txPacketBuff;
    etl::string<4 * txMaxPacketLength> packetString;
    uint16_t packetsSentCount = 0;
    uint16_t packetsReceivedCount = 0;
    uint8_t fcsOffset09 = 0; // The frame check sequence (if enabled) adds 2 or 4 octets to the end of the frame

public:
    static inline uint16_t transceiverInterruptCount = 0;
    /*
     * This function creates random packets until we have full functionality.
     */
    void createRandomPacket();

    void loadDefaultTransceiverConfiguration();
    /*
     * Power on transceiver, FPGA and external frontend. Configure transceiver.
     */
    void setupCommunicationChain(OperationalMode operationalMode);

    void communicationOperations(OperationalMode operationalMode);
    void execute();

    TransceiverTask() : Task("TransceiverTask") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<TransceiverTask>, this->TaskName,
                          TransceiverTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }

};

inline std::optional<TransceiverTask> transceiverTask;