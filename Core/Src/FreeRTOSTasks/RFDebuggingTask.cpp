#include "RfDebuggingTask.hpp"
#include "at86rf215.hpp"

void RfDebuggingTask::execute() {
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

    // non repetitive processes
    if (process == LO_CARRIER_UHF) {
        AT86RF215::transceiverUtils.transmitCarrier(AT86RF215::RF09, err);
        if (err != AT86RF215::Error::NO_ERRORS) {
            LOG_DEBUG << "[RfDebuggingTask] Failed to start transmitting UHF carrier";
        }
    } else if (process == LO_CARRIER_SBAND) {
        AT86RF215::transceiverUtils.transmitCarrier(AT86RF215::RF24, err);
        if (err != AT86RF215::Error::NO_ERRORS) {
            LOG_DEBUG << "[RfDebuggingTask] Failed to start transmitting SBAND carrier";
        }
    } else if (process == IQ_LOOPBACK_MODE) {
        AT86RF215::transceiverUtils.enableIQLoopbackMode(err);
        if (err != AT86RF215::Error::NO_ERRORS) {
            LOG_DEBUG << "[RfDebuggingTask] Failed to enable loopback mode";
        }
    }

    while (true) {
        switch (process) {
            case INTERRUPT_TEST:
                if (AT86RF215::transceiverUtils.get_state(AT86RF215::RF09, err) == AT86RF215::State::RF_TRXOFF) {
                    AT86RF215::transceiverUtils.set_state(AT86RF215::RF09, AT86RF215::State::RF_TXPREP,err);
                    LOG_DEBUG << "Just set state to TXPREP";
                } else {
                    AT86RF215::transceiverUtils.set_state(AT86RF215::RF09, AT86RF215::State::RF_TRXOFF,err);
                    LOG_DEBUG << "Just set state to TRXOFF";
                }
                break;
            case BASEBAND_TX_UHF:
                if (cyclicalCounter == 0) {
                    AT86RF215::transceiverUtils.packetTransmissionBaseband(AT86RF215::RF09, dummyPacket10, 10, err);
                    if (err != AT86RF215::Error::NO_ERRORS) {
                        LOG_DEBUG << "[RfDebuggingTask] Got error while transmitting dummyPacket10:";
                        AT86RF215::transceiverUtils.print_error(err);
                    } else {
                        LOG_DEBUG << "[RfDebuggingTask] Transmitted dummyPacket10";
                    }
                } else if (cyclicalCounter == 1) {
                    AT86RF215::transceiverUtils.packetTransmissionBaseband(AT86RF215::RF09, dummyPacket50, 50, err);
                    if (err != AT86RF215::Error::NO_ERRORS) {
                        LOG_DEBUG << "[RfDebuggingTask] Got error while transmitting dummyPacket50:";
                        AT86RF215::transceiverUtils.print_error(err);
                    } else {
                        LOG_DEBUG << "[RfDebuggingTask] Transmitted dummyPacket50";
                    }
                } else if (cyclicalCounter == 2) {
                    AT86RF215::transceiverUtils.packetTransmissionBaseband(AT86RF215::RF09, dummyPacket256, 256, err);
                    if (err != AT86RF215::Error::NO_ERRORS) {
                        LOG_DEBUG << "[RfDebuggingTask] Got error while transmitting dummyPacket256:";
                        AT86RF215::transceiverUtils.print_error(err);
                    } else {
                        LOG_DEBUG << "[RfDebuggingTask] Transmitted dummyPacket256";
                    }
                }

                cyclicalCounter = (cyclicalCounter + 1) % 3;
                break;
            case BASEBAND_RX_UHF:
                // enter "listening" state
                AT86RF215::transceiverUtils.preparePacketReceptionBaseband(AT86RF215::RF09, receivedPacket, err);
                if (err != AT86RF215::Error::NO_ERRORS) {
                    LOG_DEBUG << "[RfDebuggingTask] Got error while preparing for baseband reception";
                    AT86RF215::transceiverUtils.print_error(err);
                } else {
                    LOG_DEBUG << "[RfDebuggingTask] Entered listening mode";
                }

                // wait for a packet...
                do {
                    LOG_DEBUG << "[RfDebuggingTask] Waiting for packet...";
                    receivedPacketLength = AT86RF215::transceiverUtils.waitForPacketReceptionBaseband(AT86RF215::RF09, timeoutDelayMs, err);
                } while (err == AT86RF215::Error::RX_WAIT_TIMEOUT);

                if (err != AT86RF215::Error::NO_ERRORS) {
                    LOG_DEBUG << "[RfDebuggingTask] Got error while waiting for packet reception";
                    AT86RF215::transceiverUtils.print_error(err);
                } else {
                    LOG_DEBUG << "[RfDebuggingTask] Received packet of length: " << receivedPacketLength;
                    receivedPacketString.clear();
                    etl::string<5> num;
                    for (uint16_t i = 0; i < receivedPacketLength; i++) {
                        etl::to_string(receivedPacket[i], num);
                        receivedPacketString.append(num);
                    }
                    LOG_DEBUG << receivedPacketString.c_str();
                }
                break;
            case POWER_MEASUREMENT_UHF: {
                    int8_t powerUHF = AT86RF215::transceiverUtils.singleShotEnergyMeasurement(AT86RF215::RF09, etl::nullopt, err);
                    if (err != AT86RF215::Error::NO_ERRORS) {
                        LOG_DEBUG << "[RfDebuggingTask] Got error while trying to measure UHF power:";
                        AT86RF215::transceiverUtils.print_error(err);
                    } else {
                        LOG_DEBUG << "[RfDebuggingTask] Measured UHF power: " << powerUHF << " dBm";
                    }

                    AT86RF215::transceiverUtils.set_state(AT86RF215::RF09, AT86RF215::State::RF_TXPREP, err);
                    if (err != AT86RF215::Error::NO_ERRORS) {
                        LOG_DEBUG << "[RfDebuggingTask] Could not set state RF_TXPREP after energy measurement";
                    }
                    AT86RF215::transceiverUtils.set_state(AT86RF215::RF09, AT86RF215::State::RF_RX, err);
                    if (err != AT86RF215::Error::NO_ERRORS) {
                        LOG_DEBUG << "[RfDebuggingTask] Could not set state RF_RX after energy measurement";
                    }
                    break;
            }
            case POWER_MEASUREMENT_SBAND: {
                    int8_t powerSBAND = AT86RF215::transceiverUtils.singleShotEnergyMeasurement(AT86RF215::RF24, etl::nullopt, err);
                    if (err != AT86RF215::Error::NO_ERRORS) {
                        LOG_DEBUG << "[RfDebuggingTask] Got error while trying to measure SBAND power:";
                        AT86RF215::transceiverUtils.print_error(err);
                    } else {
                        LOG_DEBUG << "[RfDebuggingTask] Measured SBAND power: " << powerSBAND << " dBm";
                    }
                    break;
            }
            default: {}
        }
        vTaskDelay(pdMS_TO_TICKS(4000));
    }
}