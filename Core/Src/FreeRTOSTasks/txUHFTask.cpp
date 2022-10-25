#include "txUHFTask.hpp"

extern UART_HandleTypeDef huart3;

void TxUHFTask::execute() {
    bool tmEncodedBoolPacket[CodewordLength] = {0};
    uint16_t packetCount = 0;
    char log[100];
    for(;;){
        // Add packetization method
        convolutionalEncoder.encode(tmPacket, tmEncodedPacket);
        // Remove when bitset for GMSK merges
        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < CodewordLength / numSubpackets; ++i) {
                tmEncodedBoolPacket[i] = tmPacket[i];
            }
            gmskTranscoder.modulate(tmEncodedBoolPacket, CodewordLength, inPhaseBuffer, quadraturePhaseBuffer);
            // Add transceiver normalization method
            // Send to FPGA through SPI
            snprintf(log, sizeof("[%d]Sent sub-packet %d\n\r"), "[%d]Sent sub-packet %d\n\r", packetCount, j);
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(log), sizeof(log), 100);
        }
        packetCount++;
    }
}