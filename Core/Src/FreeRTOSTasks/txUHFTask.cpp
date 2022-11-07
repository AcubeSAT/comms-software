#include "txUHFTask.hpp"

extern UART_HandleTypeDef huart3;

void TxUHFTask::execute() {
    uint16_t packetCount = 0;
    char log[100];
    auto fragmentedEncodedPacket = etl::bitset<(CodewordLength * Rate) / NumberOfSubpackets>();
    for(;;){
        // Add packetization method
        convolutionalEncoder.encode(tmPacket, tmEncodedPacket);
        for (uint8_t j = 0; j < NumberOfSubpackets; ++j) {
            for (int i = 0; i < (CodewordLength * Rate) / NumberOfSubpackets; ++i) {
                fragmentedEncodedPacket.set(i, tmEncodedPacket[j * ((CodewordLength * Rate) / NumberOfSubpackets) + i]);
            }
            gmskTranscoder.modulate(fragmentedEncodedPacket, inPhaseBuffer, quadraturePhaseBuffer);
            // Add transceiver normalization method
            // Send to FPGA through SPI
            snprintf(log, sizeof("[%d]Sent sub-packet %d\n\r"), "[%d]Sent sub-packet %d\n\r", packetCount, j);
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(log), sizeof(log), 100);
        }
        packetCount++;
    }
}

