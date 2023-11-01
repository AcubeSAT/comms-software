#include "TransceiverTask.hpp"
using namespace AT86RF215;

AT86RF215::At86rf215 TransceiverTask::transceiver = AT86RF215::At86rf215(&hspi1, AT86RF215::AT86RF215Configuration());


void TransceiverTask::checkTheSPI() {
    Error err ;

    DevicePartNumber dpn = transceiver.get_part_number(err);
    switch (dpn) {
        case DevicePartNumber::AT86RF215:
            LOG_DEBUG << "part number : At86rf215";
            break;
        case DevicePartNumber::AT86RF215IQ:
            LOG_DEBUG << "part number : AT86RF215IQ";
            break;
        case DevicePartNumber::AT86RF215M:
            LOG_DEBUG << "part number : AT86RF215M";
            break;
        case DevicePartNumber::AT86RF215_INVALID:
            LOG_DEBUG << "INVALID";
            break;
    }
    /*
    if (err != NO_ERRORS) {
        // Handle the error
        switch (err) {
            case FAILED_READING_FROM_REGISTER:
                LOG_DEBUG << "FAILED READING FROM REGISTER" ;
                break;
                // Handle other error cases as needed
            default:
                LOG_DEBUG << "SOME OTHER ERROR" ;

        }
    } else {
        // No error occurred, use the 'version' value
        LOG_DEBUG << "At86rf215 Version : " ;
    }
     */
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

    uint16_t currentPacketLength = 44;
    PacketType packet = createRandomPacket(currentPacketLength);

    while (true) {
        //
        // transceiver.transmitBasebandPacketsTx(At86rf215::RF09, packet.data(), currentPacketLength, error);
        // vTaskDelay(pdMS_TO_TICKS(DelayMs));
        checkTheSPI();
        vTaskDelay(pdMS_TO_TICKS(3000));

    }
}
