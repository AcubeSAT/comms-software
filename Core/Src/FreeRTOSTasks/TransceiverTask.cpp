#include "TransceiverTask.hpp"

AT86RF215::AT86RF215 TransceiverTask::transceiver = AT86RF215::AT86RF215(&hspi1, AT86RF215::AT86RF215Configuration());

uint8_t TransceiverTask::checkTheSPI() {
    uint8_t spi_error = 0;
    AT86RF215::DevicePartNumber dpn = transceiver.get_part_number(error);
    if(dpn == AT86RF215::DevicePartNumber::AT86RF215)
        LOG_DEBUG << "SPI OK" ;
    else{
        spi_error = 1;
        LOG_DEBUG << "SPI ERROR" ;
        transceiver.chip_reset(error);
    }
    return spi_error;
}

TransceiverTask::PacketType TransceiverTask::createRandomPacket(uint16_t length) {
    PacketType packet;
    for (std::size_t i = 0; i < length; i++) {
        packet[i] = i;
    }
    return packet;
}

void TransceiverTask::setConfiguration(uint16_t pllFrequency09, uint8_t pllChannelNumber09) {
    // Sub GHz radio and BBC0
    //     General Settings
    customConfig.pllFrequency09 = pllFrequency09;
    customConfig.pllChannelNumber09 = pllChannelNumber09;
    customConfig.pllChannelMode09 = AT86RF215::PLLChannelMode::FineResolution450;
    customConfig.continuousTransmit09 = false;
    customConfig.baseBandEnable09 = true;
    customConfig.physicalLayerType09 = AT86RF215::PhysicalLayerType::BB_MRFSK;

    //     Receiver frontend
    customConfig.ifInversion09 = false;
    customConfig.ifShift09 = false;
    customConfig.rxBandwidth09 = AT86RF215::ReceiverBandwidth::RF_BW200KHZ_IF250KHZ;
    customConfig.rxRelativeCutoffFrequency09 = AT86RF215::RxRelativeCutoffFrequency::FCUT_0375;
    //      Set to FS_400 for v.1 transceiver (X-PRO) and FS_500 for v.3 transceiver (X-PRO-A)
    if (transceiver.get_version_number(error) == 3)
        customConfig.receiverSampleRate09 = AT86RF215::ReceiverSampleRate::FS_500;
    else
        customConfig.receiverSampleRate09 = AT86RF215::ReceiverSampleRate::FS_400;

    //     AGC
    customConfig.agcEnabled09 = true;
    customConfig.agcInput09 = false;
    customConfig.averageTimeNumberSamples09 = AT86RF215::AverageTimeNumberSamples::AVGS_8;
    customConfig.automaticGainControlTarget09 = AT86RF215::AutomaticGainTarget::DB30;
    customConfig.gainControlWord09 = 23;

    //     RSSI
    customConfig.energyDetectionMode09 = AT86RF215::EnergyDetectionMode::RF_EDAUTO;
    customConfig.energyDetectFactor09 = 16;
    customConfig.energyDetectionBasis09 = AT86RF215::EnergyDetectionTimeBasis::RF_8MS;

    //     Tx frontend
    customConfig.txRelativeCutoffFrequency09 = AT86RF215::TxRelativeCutoffFrequency::FCUT_0375;
    customConfig.transceiverSampleRate09 = AT86RF215::TransmitterSampleRate::FS_400;
    customConfig.powerAmplifierRampTime09 = AT86RF215::PowerAmplifierRampTime::RF_PARAMP32U;
    customConfig.transmitterCutOffFrequency09 = AT86RF215::TransmitterCutOffFrequency::RF_FLC80KHZ;
    customConfig.txOutPower09 = 0x0;  // Each increment adds 1dBm,max value is 0x1F

    //     Direct modulation and pre-emphasis filter
    //     FEATURE AVAILABLE ONLY IN V.3 TRANSCEIVER (X-PRO-A DEVBOARD)
    if (transceiver.get_version_number(error) == 3) {
        customConfig.directModulation09 = true;
        customConfig.enablePE09 = false;
        customConfig.configPE0_09 = 0x2;
        customConfig.configPE1_09 = 0x3;
        customConfig.configPE2_09 = 0xFC;
    }

    //     FSK modulation
    //     BT = 1 , MIDXS = 1, MIDX = 1, MOR = B-FSK
    transceiver.spi_write_8(AT86RF215::BBC0_FSKC0, 86, error);

    //     FCS and interleaving
    uint8_t reg = transceiver.spi_read_8(AT86RF215::BBC0_PC, error);
    //         ENABLE TXSFCS (FCS autonomously calculated)
    transceiver.spi_write_8(AT86RF215::BBC0_PC, reg | (1 << 4), error);
    //         ENABLE FCS FILTER
    transceiver.spi_write_8(AT86RF215::BBC0_PC, reg | (1 << 6), error);
    reg = transceiver.spi_read_8(AT86RF215::BBC0_FSKC2, error);
    //         DISABLE THE INTERLEAVING
    transceiver.spi_write_8(AT86RF215::BBC0_PC, reg & 0, error);


    // 2.4 GHz radio and BBC1

    transceiver.config = customConfig;
}

/*
* The frequency cannot be lower than 377000 as specified in section 6.3.2. The frequency range related
* to Fine Resolution Channel Scheme CNM.CM=1 is from 389.5MHz to 510MHz
*/
uint16_t TransceiverTask::calculatePllChannelFrequency09(uint32_t frequency) {
    uint32_t N = (frequency - 377000) * 65536 / 6500;
    return N >> 8;
}

/*
* The frequency cannot be lower than 377000 as specified in section 6.3.2. The frequency range related
* to Fine Resolution Channel Scheme CNM.CM=1 is from 389.5MHz to 510MHz
*/
uint8_t TransceiverTask::calculatePllChannelNumber09(uint32_t frequency) {
    uint32_t N = (frequency - 377000) * 65536 / 6500;
    return N & 0xFF;
}

void TransceiverTask::execute() {
    // Check SPI
    while (checkTheSPI() != 0) {
        vTaskDelay(10);
    };

    setConfiguration(calculatePllChannelFrequency09(FrequencyUHF), calculatePllChannelNumber09(FrequencyUHF));
    transceiver.chip_reset(error);
    transceiver.setup(error);
    LOG_DEBUG << "passed chip_reset and setup";

    uint16_t currentPacketLength = 44;
    PacketType packet = createRandomPacket(currentPacketLength);

    while (true) {
        LOG_DEBUG << "entered loop";

        /** Energy measurement
        transceiver.clear_channel_assessment(AT86RF215::RF09,error);           // sets the tranceiver to state RF_TXPREP (handle_irq() then sets the state
                                                                                  RF_RX and energy measurements begin)
        vTaskDelay(pdMS_TO_TICKS(3));      //wait for handle_irq() to read the measurement
        if (error!=AT86RF215::NO_ERRORS)
            LOG_DEBUG << "Error: " << static_cast<uint8_t>(error) << "\n";    // look enum at at86rf215.hpp for error values
        else
            LOG_DEBUG << "Energy (EDC register): " << transceiver.energy_measurement << "\n";    // range -127..4 dbm
            LOG_DEBUG << "Energy (RSSI register): " << transceiver.get_rssi(AT86RF215::RF09,error) << "\n";
        **/

        bool rx = true;  // Switch to false for tx.Could make that value change via pressing a button or periodically,
                         // to simulate half duplex communication
        if (rx) {
            /**RXFS,RSFE interrupts and packet reception**/
            transceiver.transmitBasebandPacketsRx(AT86RF215::RF09, error); // Sets the tranceiver to state RX
            vTaskDelay(pdMS_TO_TICKS(3));      // Wait for handle_irq() to detect the interrupts and read the packets
            if (transceiver.get_state(AT86RF215::RF09, error) != AT86RF215::RF_RX)
                LOG_DEBUG << "Could not get to state rx\n";   // Try fiddling with the delay above,if state RX cannot be reached
            else          // Flag variables got_rxfs,got_rxfe determine if interrupts occurred
                LOG_DEBUG << "Entered state rx\n";
            if (transceiver.got_rxfs)
                LOG_DEBUG << "Got rxfs\n";
            transceiver.got_rxfs = false;
            if (transceiver.got_rxfe) {
                LOG_DEBUG << "Got rxfe\n";
                transceiver.got_rxfe = false;
                // Check if there is a mistake in the packet and print it
                for (int i = 0; i < currentPacketLength; i++) {
                    if (transceiver.received_packet[i + 2] != packet.data()[i]) {
                        LOG_DEBUG << "Packet is wrong.Position: " << i;
                        break;
                    }
                    LOG_DEBUG << transceiver.received_packet[i + 2];
                }

            }
        }
        else{
            /** Tx task **/
            transceiver.transmitBasebandPacketsTx(AT86RF215::RF09, packet.data(), currentPacketLength, error);
        }

        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}