#include "TmTxDataLinkTask.hpp"
#include "Task.hpp"
#include "queue.h"



BaseType_t xStatus;

void TmTxDataLinkTask::execute(){
    uint8_t packetLength = 0;
    uint8_t packet[TmTransferFrameSize] = {0};
    uint8_t packetDestination[TmTransferFrameSize] ={0};
    BaseType_t xStatus;
    ServiceChannelNotification err;
    for(;;) {
        xStatus = xQueueReceive(transmitPacketLengthsQueue, &packetLength, pdMS_TO_TICKS(1000));
        for(uint8_t i = 0; i < packetLength ; i++){
            xStatus = xQueueReceive(transmitPacketsQueue, &packet[i], pdMS_TO_TICKS(100));
        }
        err = serviceChannelptr->storePacketTm(packet, packetLength, 0);
        if(err != NO_SERVICE_EVENT){
            char errStr[20];
            snprintf(errStr, sizeof("\n error in store packet "), "\n error in store packet ");
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errStr), sizeof(errStr), 100);
        }
        err = serviceChannelptr->vcGenerationService(60, 0);
        if(err == MEMORY_POOL_FULL){
            char errStr[20];
            snprintf(errStr, sizeof("\n error in memory pool "), "\n error in memory pool ");
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errStr), sizeof(errStr), 100);
        }
        err = serviceChannelptr->mcGenerationTMRequest();
        if(err == NO_RX_PACKETS_TO_PROCESS){
            char errStr[20];
            snprintf(errStr, sizeof("\n error: emtpy mc "), "\n error: empty mc ");
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errStr), sizeof(errStr), 100);
        }
        else if(err == TX_MC_FRAME_BUFFER_FULL){
            char errStr[20];
            snprintf(errStr, sizeof("\n error: mc full "), "\n error: mc full ");
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errStr), sizeof(errStr), 100);
        }
        err = serviceChannelptr->allFramesGenerationTMRequest(packetDestination, TmTransferFrameSize);
        if(err != NO_SERVICE_EVENT){
            char errStr[20];
            snprintf(errStr, sizeof("\n error in allFrames "), "\n error in allFrames ");
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errStr), sizeof(errStr), 100);
        }

        etl::string<TmTransferFrameSize + 30> string = "\r\n Transfer Frame data bytes: ";
        etl::format_spec format;
//        char str1[30];
//        char str2[3] = {0};
//        char str3[18];
//        snprintf(str1, sizeof("\r\n Transfer Frame data bytes: "), "\r\n Transfer Frame data bytes: ");
//        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(str1), sizeof(str1), 100);
        for(uint8_t i = 0; i < packetLength + 10 ; i++){
//            snprintf(str2, sizeof(" %d ")," %d ", packetDestination[i]);
//            HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t *> (str2), sizeof(str2), 100);
            etl::to_string(packetDestination[i], string, format, true);
            string.append(" ");
        }
//        snprintf(str3, sizeof(" Packet Length: %d ")," Packet Length: %d ", packetLength);
        string.append(" Packet Length: ");
        etl::to_string(packetLength, packetLength, string, format, true);
        string.append(" \n ");
        HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t *>(string.data()), sizeof(string), 100);

        HAL_Delay(1000);
    }

}
