#include "TransceiverTask.h"

/*
 * This function creates random packets until we have full functionality.
 */
void TransceiverTask::createRandomPacket(etl::array<uint8_t, MaxPacketLength> &packet, uint16_t length){
    for (int i=0; i<length; i++){
        packet[i] = i;
    }
}

void TransceiverTask::execute() {

    transceiver.chip_reset(error);
    transceiver.setup(error);
    volatile uint16_t  b = transceiver.get_version_number(error);

    uint16_t currentPacketLength = 24;
    etl::array<uint8_t, MaxPacketLength> packet;
    createRandomPacket(packet, currentPacketLength);

    for (int i = 0; i < MaxPacketLength; i++) {
        xQueueSendToBack(packetQueue, &packet[i], portMAX_DELAY);
    }

    uint8_t receivedPacket[MaxPacketLength];
    for (int i = 0; i < currentPacketLength; i++) {
        xQueueReceive(packetQueue, &receivedPacket[i], portMAX_DELAY);
    }

    while (true) {

        volatile AT86RF215::State state = transceiver.get_state(AT86RF215::RF09, error);
        state;
        transceiver.transmitBasebandPacketsTx(AT86RF215::RF09, receivedPacket, currentPacketLength, error);
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }

}

