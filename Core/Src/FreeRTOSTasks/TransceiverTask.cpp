#include "TransceiverTask.hpp"
using namespace AT86RF215;

AT86RF215::At86rf215 TransceiverTask::transceiver = AT86RF215::At86rf215(&hspi1, AT86RF215::AT86RF215Configuration());

uint8_t TransceiverTask::checkTheSPI() {
    uint8_t error_t = 0;
    Error err;
    DevicePartNumber dpn = transceiver.get_part_number(err);
    switch (dpn) {
        case DevicePartNumber::AT86RF215:
            LOG_DEBUG << "Part Number : AT86RF215" ;
            break;
        case DevicePartNumber::AT86RF215IQ:
            LOG_DEBUG << "Part Number : AT86RF215IQ" ;
            break;
        case DevicePartNumber::AT86RF215M:
            LOG_DEBUG << "Part Number : AT86RF215M" ;
            break;
        case DevicePartNumber::AT86RF215_INVALID:
            LOG_DEBUG << "Invalid";
            error_t = 1;
            break;
    }
    if(err != NO_ERRORS){
        error_t = 1;
        switch (err) {
            case FAILED_READING_FROM_REGISTER:
                LOG_DEBUG << "Failed reading from register" ;
                break;
            default:
                LOG_DEBUG << "some other error";
        }
    }
    if(error_t == 1)
        transceiver.chip_reset(err);
    return error_t;
}

TransceiverTask::PacketType TransceiverTask::createRandomPacket(uint16_t length) {
    PacketType packet;
    for (std::size_t i = 0; i < length; i++) {
        packet[i] = i;
    }
    return packet;
}


void TransceiverTask::setConfiguration(uint16_t pllFrequency09, uint8_t pllChannelNumber09) {
    configFrequency.pllFrequency09 = pllFrequency09;
    configFrequency.pllChannelNumber09 = pllChannelNumber09;
    configFrequency.pllChannelMode09 = AT86RF215::PLLChannelMode::FineResolution450;
    transceiver.config = configFrequency;
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
    setConfiguration(calculatePllChannelFrequency09(FrequencyUHF), calculatePllChannelNumber09(FrequencyUHF));
    transceiver.chip_reset(error);
    transceiver.setup(error);

    uint16_t currentPacketLength = 64;
    PacketType packet = createRandomPacket(currentPacketLength);

    while(checkTheSPI() != 0);
    RegisterAddress regphy;
    regphy = AT86RF215::BBC0_FSKC0;

    // BBC0_FSKC0 REGISTER
    //transceiver.spi_write_8(BBC0_FSKC0,210,error); // BT = 2, MINDEX = 0.5, 2-FSK
    //transceiver.spi_write_8(BBC0_FSKC0,208,error); // BT = 2, MINDEX = 0.375, 2-FSK
    transceiver.spi_write_8(BBC0_FSKC0,82,error);  // BT = 1, MINDEX = 0.5, MINDEX = 0.5, 2-FSK


    // set direct mod ON

    transceiver.set_direct_modulation(RF09, 1, error);
    uint8_t temp = (transceiver.spi_read_8(BBC0_FSKDM, error)) & 0b11111110;
    // set the EN to 1
    transceiver.spi_write_8(BBC0_FSKDM, temp |  0b00000001, error);
    //uint8_t reg = transceiver.spi_read_8(regphy, error);


    while (true){
        transceiver.transmitBasebandPacketsTx(AT86RF215::RF09, packet.data(), currentPacketLength, error);
        LOG_DEBUG << "signal transmitted";
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
