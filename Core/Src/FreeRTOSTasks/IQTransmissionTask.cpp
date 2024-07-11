#include "IQTransmissionTask.hpp"

AT86RF215::AT86RF215 IQTransmissionTask::transceiver = AT86RF215::AT86RF215(&hspi1, AT86RF215::AT86RF215Configuration());
//
//void IQTransmissionTask::setConfiguration(uint16_t pllFrequency09, uint8_t pllChannelNumber09) {
//    // Frequency settings
//    customConfig.pllFrequency09 = pllFrequency09;
//    customConfig.pllChannelNumber09 = pllChannelNumber09;
//    customConfig.pllChannelMode09 = AT86RF215::PLLChannelMode::FineResolution450;
//
//    //     FSK modulation
//    //     BT = 1 , MIDXS = 1, MIDX = 1, MOR = B-FSK
//    transceiver.spi_write_8(AT86RF215::BBC0_FSKC0, 86, error);
//
//    //     FCS and interleaving
//    uint8_t reg = transceiver.spi_read_8(AT86RF215::BBC0_PC, error);
//    //         ENABLE TXSFCS (FCS autonomously calculated)
//    transceiver.spi_write_8(AT86RF215::BBC0_PC, reg | (1 << 4), error);
//    //         ENABLE FCS FILTER
//    transceiver.spi_write_8(AT86RF215::BBC0_PC, reg | (1 << 6), error);
//    reg = transceiver.spi_read_8(AT86RF215::BBC0_FSKC2, error);
//    //         DISABLE THE INTERLEAVING
//    transceiver.spi_write_8(AT86RF215::BBC0_PC, reg & 0, error);
//
//
//    // 2.4 GHz radio and BBC1
//
//    transceiver.config = customConfig;
//}

void IQTransmissionTask::setConfiguration(uint16_t pllFrequency24, uint8_t pllChannelNumber24) {
    configFrequency.pllFrequency24 = pllFrequency24;
    configFrequency.pllChannelNumber24 = pllChannelNumber24;
    configFrequency.pllChannelMode24 = AT86RF215::PLLChannelMode::FineResolution2443;
    transceiver.config = configFrequency;
}

/*
* The frequency cannot be lower than 377000 as specified in section 6.3.2. The frequency range related
* to Fine Resolution Channel Scheme CNM.CM=1 is from 389.5MHz to 510MHz
*/
uint16_t IQTransmissionTask::calculatePllChannelFrequency24(uint32_t frequency) {
    uint32_t N = (frequency - 2366000) * 65536 / 26000;
    return N >> 8;
}

/*
* The frequency cannot be lower than 377000 as specified in section 6.3.2. The frequency range related
* to Fine Resolution Channel Scheme CNM.CM=1 is from 389.5MHz to 510MHz
*/
uint8_t IQTransmissionTask::calculatePllChannelNumber24(uint32_t frequency) {
    uint32_t N = (frequency - 2366000) * 65536 / 26000;
    return N & 0xFF;
}

void IQTransmissionTask::execute() {
    setConfiguration(calculatePllChannelFrequency24(FrequencyS), calculatePllChannelNumber24(FrequencyS));
    transceiver.chip_reset(error);
    transceiver.setup(error);
//    transceiver.spi_write_8(AT86RF215::RegisterAddress::RF_IQIFC1, 0b00010010, error);
    transceiver.spi_write_8(AT86RF215::RegisterAddress::RF24_PADFE, 2 << 6, error);
//    transceiver.spi_write_8(AT86RF215::RegisterAddress::RF09_PADFE, 2 << 6, error);
    LOG_DEBUG << "passed chip_reset and setup";

//    transceiver.spi_write_8(AT86RF215::RegisterAddress::RF09_PADFE, 2 << 6, error);
    while (true) {
        transceiver.transmitPacketsIQ(AT86RF215::RF24, true, error);
//        transceiver.transmitPacketsIQ(AT86RF215::RF09, true, error);
        LOG_DEBUG << "interrupt count: " << interruptCount;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}