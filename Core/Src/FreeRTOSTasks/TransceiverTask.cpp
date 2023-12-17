#include "TransceiverTask.hpp"

AT86RF215::AT86RF215 TransceiverTask::transceiver = AT86RF215::AT86RF215(&hspi1, AT86RF215::AT86RF215Configuration());


TransceiverTask::PacketType TransceiverTask::createRandomPacket(uint16_t length) {
    PacketType packet;
    for (std::size_t i = 0; i < length; i++) {
        packet[i] = i;
    }
    return packet;
}

void TransceiverTask::setConfiguration(uint16_t pllFrequency24, uint8_t pllChannelNumber24) {
    configFrequency.pllFrequency24 = pllFrequency24;
    configFrequency.pllChannelNumber24 = pllChannelNumber24;
    configFrequency.pllChannelMode24 = AT86RF215::PLLChannelMode::FineResolution2443;
    transceiver.config = configFrequency;
}

/*
* The frequency cannot be lower than 377000 as specified in section 6.3.2. The frequency range related
* to Fine Resolution Channel Scheme CNM.CM=1 is from 389.5MHz to 510MHz
*/
uint16_t TransceiverTask::calculatePllChannelFrequency24(uint32_t frequency) {
    uint32_t N = (frequency - 2366000) * 65536 / 26000;
    return N >> 8;
}

/*
* The frequency cannot be lower than 377000 as specified in section 6.3.2. The frequency range related
* to Fine Resolution Channel Scheme CNM.CM=1 is from 389.5MHz to 510MHz
*/
uint8_t TransceiverTask::calculatePllChannelNumber24(uint32_t frequency) {
    uint32_t N = (frequency - 2366000) * 65536 / 26000;
    return N & 0xFF;
}

void TransceiverTask::execute() {
    setConfiguration(calculatePllChannelFrequency24(FrequencyUHF), calculatePllChannelNumber24(FrequencyUHF));

    transceiver.chip_reset(error);
    transceiver.setup(error);

    uint16_t currentPacketLength = 44;
    PacketType packet = createRandomPacket(currentPacketLength);

    while (true) {
        transceiver.transmitBasebandPacketsTx(AT86RF215::RF24, packet.data(), currentPacketLength, error);
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
