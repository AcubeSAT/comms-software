#include "txUHFTask.hpp"
#include "UARTGatekeeperTask.hpp"
extern UART_HandleTypeDef huart3;
void TxUHFTask::execute() {
    uint16_t packetCount = 0;
    char log[100];
    auto fragmentedEncodedPacket = etl::bitset<(CodewordLength * Rate) / NumberOfSubpackets>();
    uint32_t start_time, end_time, total_time = 0;
    for(;;){
//        // Add packetization method
//        convolutionalEncoder.encode(tmPacket, tmEncodedPacket);
//        for (uint8_t j = 0; j < NumberOfSubpackets; ++j) {
//            for (int i = 0; i < (CodewordLength * Rate) / NumberOfSubpackets; ++i) {
//                fragmentedEncodedPacket.set(i, tmEncodedPacket[j * ((CodewordLength * Rate) / NumberOfSubpackets) + i]);
//            }
            start_time = HAL_GetTick();
            gmskTranscoder.modulate(tmPacket, inPhaseBuffer, quadraturePhaseBuffer);
            end_time = HAL_GetTick();
            total_time = end_time - start_time;

            etl::string<32> str1 = "GMSK Modulate1: ";
            etl::string<32> str2 = "GMSK DeModulate1: ";
            etl::string<10> breakline = "\r\n";
            uartGatekeeperTask->addToQueue(etl::to_string(total_time, str1, true));
            uartGatekeeperTask->addToQueue(breakline);

//            start_time = HAL_GetTick();
//            gmskTranscoder.demodulate(inPhaseBuffer, quadraturePhaseBuffer,
//                                      sizeof(inPhaseBuffer)/sizeof(inPhaseBuffer[0]), tmPacket);
//            end_time = HAL_GetTick();
//            total_time = end_time - start_time;
//            uartGatekeeperTask->addToQueue(etl::to_string(total_time, str2, true));
//            uartGatekeeperTask->addToQueue(breakline);
//            uartGatekeeperTask->addToQueue(breakline);
//            uartGatekeeperTask->addToQueue("NEW");
//            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>("[%d]Sent sub-packet %d\n\r"), 10, 100);
//            LOG_DEBUG<<"[%d]Sent sub-packet %d\n\r";

        //}
        packetCount++;
    }
}

