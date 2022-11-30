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
        etl::string<200> string = "\r Transfer Frame data bytes: ";
        etl::format_spec format;
        for(uint8_t i = 0; i < packetLength + 10 ; i++){
            etl::to_string(packetDestination[i], string, format, true);
            string.append(" ");
        }
        string.append(" Packet Length: ");
        etl::to_string(packetLength, string, format, true);
        string.append("\n");
        HAL_UART_Transmit(&huart3, (uint8_t*)(string.data()), string.size(), 100);
        HAL_Delay(1000);
    }
}
