#include "TransceiverBasebandCoreTask.hpp"
#include "at86rf215.hpp"

void TransceiverBasebandCoreTask::execute() {
    // initialize dummy packets with various sizes
    for (uint8_t i = 0; i < 10; i++) {
        dummyPacket10[i] = i;
    }

    for (uint8_t i = 0; i < 50; i++) {
        dummyPacket50[i] = i;
    }

    for (uint16_t i = 0; i < 256; i++) {
        dummyPacket256[i] = i;
    }

    AT86RF215::Error err;
    while (true) {
        switch (process) {
            case (BASEBAND_TX):
                if (cyclicalCounter == 0) {
                    AT86RF215::transceiverUtils.packetTransmissionBaseband(AT86RF215::RF09, dummyPacket10, 10, err);
                    if (err != AT86RF215::Error::NO_ERRORS) {
                        LOG_DEBUG << "[TransceiverBasebandCoreTask] Got error while transmitting dummyPacket10:";
                        AT86RF215::transceiverUtils.print_error(err);
                    } else {
                        LOG_DEBUG << "[TransceiverBasebandCoreTask] Transmitted dummyPacket10";
                    }
                } else if (cyclicalCounter == 1) {
                    AT86RF215::transceiverUtils.packetTransmissionBaseband(AT86RF215::RF09, dummyPacket50, 50, err);
                    if (err != AT86RF215::Error::NO_ERRORS) {
                        LOG_DEBUG << "[TransceiverBasebandCoreTask] Got error while transmitting dummyPacket50:";
                        AT86RF215::transceiverUtils.print_error(err);
                    } else {
                        LOG_DEBUG << "[TransceiverBasebandCoreTask] Transmitted dummyPacket50";
                    }
                } else if (cyclicalCounter == 2) {
                    AT86RF215::transceiverUtils.packetTransmissionBaseband(AT86RF215::RF09, dummyPacket256, 256, err);
                    if (err != AT86RF215::Error::NO_ERRORS) {
                        LOG_DEBUG << "[TransceiverBasebandCoreTask] Got error while transmitting dummyPacket256:";
                        AT86RF215::transceiverUtils.print_error(err);
                    } else {
                        LOG_DEBUG << "[TransceiverBasebandCoreTask] Transmitted dummyPacket256";
                    }
                }

                cyclicalCounter = (cyclicalCounter + 1) % 3;
            case (BASEBAND_RX):
                // enter "listening" state
                AT86RF215::transceiverUtils.preparePacketReceptionBaseband(AT86RF215::RF09, receivedPacket, err);
                if (err != AT86RF215::Error::NO_ERRORS) {
                    LOG_DEBUG << "[TransceiverBasebandCoreTask] Got error while preparing for baseband reception";
                    AT86RF215::transceiverUtils.print_error(err);
                } else {
                    LOG_DEBUG << "[TransceiverBasebandCoreTask] Entered listening mode";
                }

                // wait for a packet...
                receivedPacketLength = AT86RF215::transceiverUtils.waitForPacketReceptionBaseband(AT86RF215::RF09, err);
                if (err != AT86RF215::Error::NO_ERRORS) {
                    LOG_DEBUG << "[TransceiverBasebandCoreTask] Got error while waiting for packet reception";
                    AT86RF215::transceiverUtils.print_error(err);
                } else {
                    LOG_DEBUG << "[TransceiverBasebandCoreTask] Received packet of length: " << receivedPacketLength;
                    receivedPacketString.clear();
                    etl::string<5> num;
                    for (uint16_t i = 0; i < receivedPacketLength; i++) {
                        etl::to_string(receivedPacket[i], num);
                        receivedPacketString.append(num);
                    }
                    LOG_DEBUG << receivedPacketString.c_str();
                }
            case (POWER_MEASUREMENT):
                int8_t power = AT86RF215::transceiverUtils.clear_channel_assessment(AT86RF215::RF09, etl::nullopt, err);
                if (err != AT86RF215::Error::NO_ERRORS) {
                    LOG_DEBUG << "[TransceiverBasebandCoreTask] Got error while trying to measure power:";
                    AT86RF215::transceiverUtils.print_error(err);
                } else {
                    LOG_DEBUG << "[TransceiverBasebandCoreTask] Measured power: " << power << " dBm";
                }
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}