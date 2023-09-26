#include "TransceiverTask.hpp"

AT86RF215::AT86RF215 TransceiverTask::transceiver = AT86RF215::AT86RF215(&hspi1, AT86RF215::AT86RF215Configuration());



TransceiverTask::Packet TransceiverTask::createRandomPacket(uint16_t length) {
    Packet packet;
    for (int i = 0; i < length; i++) {
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

uint16_t TransceiverTask::calculatePllChannelFrequency09(uint32_t frequency) {
    uint32_t N = (frequency - 377000) * 65536 / 6500;
    return N >> 8;
}

uint8_t TransceiverTask::calculatePllChannelNumber09(uint32_t frequency) {
    uint32_t N = (frequency - 377000) * 65536 / 6500;
    return N & 0xFF;
}

void TransceiverTask::execute() {
    setConfiguration(calculatePllChannelFrequency09(FrequencyUHF), calculatePllChannelNumber09(FrequencyUHF));

    transceiver.chip_reset(error);
    transceiver.setup(error);

    uint16_t currentPacketLength = 44;
    Packet packet;
    packet = createRandomPacket(currentPacketLength);


    for (int i = 0; i < MaxPacketLength; i++) {
        xQueueSendToBack(packetQueue, &packet[i], portMAX_DELAY);
    }

    while (true) {
        transceiver.transmitBasebandPacketsTx(AT86RF215::RF09, packet.data(), currentPacketLength, error);
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
