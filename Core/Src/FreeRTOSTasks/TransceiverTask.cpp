#include "TransceiverTask.hpp"

AT86RF215::At86rf215 transceiver = AT86RF215::At86rf215(&hspi1);

void TransceiverTask::createRandomPacket() {
    static uint8_t currPacketNum = 0;
    static etl::array<uint8_t, 4> packetSizes = {10, 20, 30, 200};

    // cycle through packet sizes
    currPacketNum = (currPacketNum == 3) ? 0 : currPacketNum + 1;

    packetString.clear();
    for (std::size_t i = 0; i < packetSizes[currPacketNum]; i++) {
        txPacketBuff[i] = i;
        packetString.append(std::to_string(i).c_str());
        packetString.append(" ");
    }
    txPacketLength = packetSizes[currPacketNum] + fcsOffset09;
    uint8_t test  = txPacketLength;
}

void TransceiverTask::loadDefaultTransceiverConfiguration() {
    transceiver.setGeneralConfig(AT86RF215::GeneralConfiguration::DefaultGeneralConfig());
    transceiver.setRXConfig(AT86RF215::RXConfig::DefaultRXConfig());
    transceiver.setTXConfig(AT86RF215::TXConfig::DefaultTXConfig());
    transceiver.setBaseBandCoreConfig(AT86RF215::BasebandCoreConfig::DefaultBasebandCoreConfig());
    transceiver.setFrequencySynthesizerConfig(AT86RF215::FrequencySynthesizer::DefaultFrequencySynthesizerConfig());
    transceiver.setExternalFrontEndControlConfig(AT86RF215::ExternalFrontEndConfig::DefaultExternalFrontEndConfig());
    transceiver.setInterruptConfig(
            AT86RF215::BasebandCoreInterruptsConfig::DefaultBasebandCoreInterruptsConfig());
    transceiver.setRadioInterruptConfig(AT86RF215::RadioInterruptsConfig::DefaultRadioInterruptsConfig());
    transceiver.setIQInterfaceConfig(AT86RF215::IQInterfaceConfig::DefaultIQInterfaceConfig());
}

void TransceiverTask::setupCommunicationChain(OperationalMode operationalMode) {
    switch (operationalMode) {
        case TRANSCEIVER_PROCESSING_UHF_TXRX:
            // peripheral power supply and external RF frontend setup
//            HAL_GPIO_WritePin(P5V_RF_EN_GPIO_Port, P5V_RF_EN_Pin, GPIO_PIN_SET); // 5V RF power supply ON (includes transceiver)
//            HAL_GPIO_WritePin(P5V_FPGA_EN_GPIO_Port, P5V_FPGA_EN_Pin, GPIO_PIN_RESET); // FPGA power supply OFF
//
//            HAL_GPIO_WritePin(EN_S_BAND_TX_GPIO_Port,EN_S_BAND_TX_Pin, GPIO_PIN_SET); // SBAND RF current limiter OFF
//            HAL_GPIO_WritePin(EN_RX_UHF_GPIO_Port, EN_RX_UHF_Pin, GPIO_PIN_RESET); // UHF RF current limiters ON
//            HAL_GPIO_WritePin(EN_PA_UHF_GPIO_Port, EN_PA_UHF_Pin, GPIO_PIN_RESET);

            // transceiver setup
            //     load stored default configuration
            loadDefaultTransceiverConfiguration();
            //     turn off IQ interface
            transceiver.iqInterfaceConfig.set_RF_IQIFC1(AT86RF215::ChipMode::RF_MODE_BBRF, AT86RF215::SkewAlignment::SKEW3906NS);
            //     turn off BBC1 (2.4 GHz baseband core)
            transceiver.basebandCoreConfig.setBBC_PC(AT86RF215::Transceiver::RF24, false, true, false, AT86RF215::FrameCheckSequenceType::FCS_32, false, AT86RF215::PhysicalLayerType::BB_OFF);

            transceiver.setup(transceiverError);
            if (transceiverError != AT86RF215::Error::NO_ERRORS) {
                LOG_DEBUG << "Error during transceiver setup";
            }
            //     set 2.4 GHz radio to sleep mode (low power consumption)
            transceiver.set_state(AT86RF215::RF24, AT86RF215::State::RF_SLEEP, transceiverError);
            break;
        case FPGA_PROCESSING_SBAND_TX:
            // peripheral power supply and external RF frontend setup
//            HAL_GPIO_WritePin(P5V_RF_EN_GPIO_Port, P5V_RF_EN_Pin, GPIO_PIN_SET); // 5V RF power supply ON (includes transceiver)
//            HAL_GPIO_WritePin(P5V_FPGA_EN_GPIO_Port, P5V_FPGA_EN_Pin, GPIO_PIN_SET); // FPGA power supply ON
//
//            HAL_GPIO_WritePin(EN_S_BAND_TX_GPIO_Port,EN_S_BAND_TX_Pin, GPIO_PIN_RESET); // SBAND RF current limiter ON
//            HAL_GPIO_WritePin(EN_RX_UHF_GPIO_Port, EN_RX_UHF_Pin, GPIO_PIN_SET); // UHF RF current limiters OFF
//            HAL_GPIO_WritePin(EN_PA_UHF_GPIO_Port, EN_PA_UHF_Pin, GPIO_PIN_SET);

            // transceiver setup
            //     load stored default configuration
            loadDefaultTransceiverConfiguration();

            transceiver.setup(transceiverError);
            if (transceiverError != AT86RF215::Error::NO_ERRORS) {
                LOG_DEBUG << "Error during transceiver setup";
            }
            //     set sub GHz radio to sleep mode (low power consumption)
            transceiver.set_state(AT86RF215::RF09, AT86RF215::State::RF_SLEEP, transceiverError); // set

            // fpga setup (if needed)
            break;
        case FPGA_PROCESSING_UHF_TXRX:
            // @TODO
            break;
        case FPGA_PROCESSING_LOOPBACK:
            // peripheral power supply and external RF frontend setup
//            HAL_GPIO_WritePin(P5V_RF_EN_GPIO_Port, P5V_RF_EN_Pin, GPIO_PIN_SET); // 5V RF power supply ON (includes transceiver)
//            HAL_GPIO_WritePin(P5V_FPGA_EN_GPIO_Port, P5V_FPGA_EN_Pin, GPIO_PIN_SET); // FPGA power supply ON
//
//            HAL_GPIO_WritePin(EN_S_BAND_TX_GPIO_Port,EN_S_BAND_TX_Pin, GPIO_PIN_SET); // SBAND RF current limiter OFF
//            HAL_GPIO_WritePin(EN_RX_UHF_GPIO_Port, EN_RX_UHF_Pin, GPIO_PIN_SET); // UHF RF current limiters OFF
//            HAL_GPIO_WritePin(EN_PA_UHF_GPIO_Port, EN_PA_UHF_Pin, GPIO_PIN_SET);

            // transceiver setup
            //     load stored default configuration
            loadDefaultTransceiverConfiguration();
            //     enable loopback
            transceiver.iqInterfaceConfig.set_RF_IQIFC0(AT86RF215::ExternalLoopback::ENABLED,
                                                        AT86RF215::IQOutputCurrent::CURR_2_MA,
                                                        AT86RF215::IQmodeVoltage::MODE_150_MV,
                                                        AT86RF215::IQmodeVoltageIEE::IEEE,
                                                        AT86RF215::EmbeddedControlTX::ENABLED);

            transceiver.setup(transceiverError);
            if (transceiverError != AT86RF215::Error::NO_ERRORS) {
                LOG_DEBUG << "Error during transceiver setup";
            }

            // fpga setup (if needed)
            break;
        case SLEEP:
//            HAL_GPIO_WritePin(P5V_RF_EN_GPIO_Port, P5V_RF_EN_Pin, GPIO_PIN_RESET); // RF power supply OFF (includes transceiver)
//            HAL_GPIO_WritePin(P5V_FPGA_EN_GPIO_Port, P5V_FPGA_EN_Pin, GPIO_PIN_RESET); // FPGA power supply OFF
            break;
    }
}

void TransceiverTask::communicationOperations(OperationalMode operationalMode) {

    switch (operationalMode) {
        case TRANSCEIVER_PROCESSING_UHF_TXRX:
            if (rxTx) {
                // tx operations
                // TXFE interrupt was raised -> successful transmission occurred. Print message and reset flag
                if (transceiver.TransmitterFrameEnd_flag) {
                    LOG_DEBUG << "Sent packet: " << packetString.c_str();
                    LOG_DEBUG << "Total packets sent count: " << ++packetsSentCount;
                    transceiver.TransmitterFrameEnd_flag = false;
                }

                // transceiver is ready to transmit
                if (!(transceiver.tx_ongoing || transceiver.rx_ongoing || transceiver.cca_ongoing)) {
                    createRandomPacket();
                    transceiver.packetTransmissionBaseband(AT86RF215::RF09, txPacketBuff.data(), txPacketLength + fcsOffset09,
                                                           transceiverError);
                    if (transceiverError != AT86RF215::NO_ERRORS) {
                        LOG_DEBUG << "Transceiver error during transmission";
                    }
                }
            }
            else {
                // rx operations
                // RXFE interrupt was raised -> successful reception occurred. Read the packet, print a message and reset the flag
                if (transceiver.ReceiverFrameEnd_flag) {
                    transceiver.packetReceptionBaseband(AT86RF215::RF09, transceiverError);
                    for (uint16_t i = 0; i < transceiver.received_packet_length - fcsOffset09; i++) {
                        packetString.append(std::to_string(transceiver.received_packet[i]).c_str());
                    }
                    LOG_DEBUG << "Received packet of length " << transceiver.received_packet_length - fcsOffset09 << ": "  << packetString.c_str();
                    LOG_DEBUG << "Total packets received: " << ++packetsReceivedCount;
                    transceiver.ReceiverFrameEnd_flag = false;
                }

                // transceiver is ready to receive
                if (!(transceiver.tx_ongoing || transceiver.rx_ongoing || transceiver.cca_ongoing)) {
                    transceiver.beginBasebandPacketReception(AT86RF215::RF09, transceiverError);
                    if (transceiverError != AT86RF215::NO_ERRORS) {
                        LOG_DEBUG << "Transceiver error while preparing for reception";
                    }
                }
            }

            break;
        case FPGA_PROCESSING_SBAND_TX:
            // @TODO Send frames to FPGA via spi. Due to the embedded control feature, no other action needs to be taken
            if (transceiver.get_iqSyncStatus(transceiverError)) {
                LOG_DEBUG << "IQ synchronization detected. Total detection count: " << ++iqSyncCount;
            }
            break;
        case FPGA_PROCESSING_UHF_TXRX:
            // @TODO
            break;
        case FPGA_PROCESSING_LOOPBACK:
            if (transceiver.get_iqSyncStatus(transceiverError)) {
                LOG_DEBUG << "IQ synchronization detected. Total detection count: " << ++iqSyncCount;
            }
            break;
        case SLEEP:
            LOG_DEBUG << "No communications";
            break;
    }
}

void TransceiverTask::execute() {
    opMode = TRANSCEIVER_PROCESSING_UHF_TXRX;

    // relevant for transceiver baseband processing only
    rxTx = true;
    if (transceiver.basebandCoreConfig.frameCheckSequenceFilterEn09) {
        if (transceiver.basebandCoreConfig.frameCheckSequenceType09 == AT86RF215::FrameCheckSequenceType::FCS_32) {
            fcsOffset09 = 4;
        }
        else {
            fcsOffset09 = 2;
        }
    }

    setupCommunicationChain(opMode);
    while (true) {
        // monitor transceiver's supply voltage
//        if (transceiver.BatteryLow_flag) {
//            LOG_DEBUG << "LOW BATTERY";
//            transceiver.BatteryLow_flag = false;
//        }

        if (transceiver.get_battery_monitor_status(transceiverError) == AT86RF215::BatteryMonitorStatus::ABOVE_THRESHOLD) {
            LOG_DEBUG << "Transceiver voltage above set threshold (1.8 V)";
        }
        else {
            LOG_DEBUG << "Transceiver voltage below set threshold (1.8 V)";
        }

        // Track total interrupts
        LOG_DEBUG << "Current interrupt count: " << transceiverInterruptCount;

        communicationOperations(opMode);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}