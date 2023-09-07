#include "TransceiverTask.hpp"

AT86RF215::AT86RF215 TransceiverTask::transceiver = AT86RF215::AT86RF215(&hspi1, AT86RF215::AT86RF215Configuration());


/*
 * This function creates random packets until we have full functionality.
 */
void TransceiverTask::createRandomPacket(etl::array<uint8_t, MaxPacketLength> &packet, uint16_t length) {
    for (int i = 0; i < length; i++) {
        packet[i] = i;
    }
}

void TransceiverTask::setConfiguration(uint16_t pllFrequency09, uint8_t pllChannelNumber09)
{
    configFrequency.pllFrequency09 = pllFrequency09;
    configFrequency.pllChannelNumber09 = pllChannelNumber09;
    configFrequency.pllChannelMode09 = AT86RF215::PLLChannelMode::FineResolution450;
    transceiver.config = configFrequency;
}

void TransceiverTask::execute() {
    volatile uint16_t b = transceiver.get_version_number(error);
    setConfiguration(PllFrequency09, PllChannelNumber09);
    transceiver.chip_reset(error);
    transceiver.setup(error);

    uint16_t currentPacketLength = 44;
    etl::array<uint8_t, MaxPacketLength> packet;
    createRandomPacket(packet, currentPacketLength);


    for (int i = 0; i < MaxPacketLength; i++) {
        xQueueSendToBack(packetQueue, &packet[i], portMAX_DELAY);
    }

    uint8_t receivedPacket[MaxPacketLength];
    for (int i = 0; i < currentPacketLength; i++) {
        xQueueReceive(packetQueue, &receivedPacket[i], portMAX_DELAY);
    }
    volatile uint8_t ccf0l = transceiver.spi_read_8(AT86RF215::RF09_CCF0L, error);
    volatile uint8_t ccf0h = transceiver.spi_read_8(AT86RF215::RF09_CCF0H, error);
    volatile uint8_t cnl = transceiver.spi_read_8(AT86RF215::RF09_CNL, error);
    volatile uint32_t N = (ccf0h << 16) | (ccf0l << 8) | (cnl);
    volatile uint32_t freq = 377000 + 6500 * N / 65536;

    //    volatile uint32_t pllfreq = N >> 8;
//    volatile uint32_t newN = (436500 - 377000) * 65536  / 6500;
//    volatile uint32_t newPllfreq = newN >> 8;
    transceiver.set_pll_channel_frequency(AT86RF215::RF09, 2343, error);

    while (true) {

        volatile AT86RF215::State state = transceiver.get_state(AT86RF215::RF09, error);
        state;
        transceiver.transmitBasebandPacketsTx(AT86RF215::RF09, receivedPacket, currentPacketLength, error);
        vTaskDelay(pdMS_TO_TICKS(DelayMs));  // Use FreeRTOS's delay function
    }
}
