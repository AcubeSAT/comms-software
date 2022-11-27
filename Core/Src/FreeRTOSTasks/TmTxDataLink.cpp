#include "TmTxDataLinkTask.hpp"
#include "Task.hpp"
#include "queue.h"



BaseType_t xStatus;

void TmTxDataLinkTask::execute(){
    char index = 0;
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
            snprintf(errStr, sizeof("\n problema in store packet "), "\n problema in store packet ");
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errStr), sizeof(errStr), 100);
        }
        err = serviceChannelptr->vcGenerationService(60, 0);
        if(err == MEMORY_POOL_FULL){
            char errStr[20];
            snprintf(errStr, sizeof("\n problema in memory pool "), "\n problema in memory pool ");
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errStr), sizeof(errStr), 100);
        }
        err = serviceChannelptr->mcGenerationTMRequest();
        if(err == NO_RX_PACKETS_TO_PROCESS){
            char errStr[20];
            snprintf(errStr, sizeof("\n problema: adeio mc "), "\n problema: adeio mc ");
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errStr), sizeof(errStr), 100);
        }
        else if(err == TX_MC_FRAME_BUFFER_FULL){
            char errStr[20];
            snprintf(errStr, sizeof("\n problema gemato mc "), "\n problema gemato mc ");
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errStr), sizeof(errStr), 100);
        }
        err = serviceChannelptr->allFramesGenerationTMRequest(packetDestination, TmTransferFrameSize);
        if(err != NO_SERVICE_EVENT){
            char errStr[20];
            snprintf(errStr, sizeof("\n problema in allFrames "), "\n problema in allFrames ");
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errStr), sizeof(errStr), 100);
        }


        char str1[30];
        char str2[3] = {0};
        char str3[18];
        snprintf(str1, sizeof("\r\n Transfer Frame data bytes: "), "\r\n Transfer Frame data bytes: ");
        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(str1), sizeof(str1), 100);
        for(uint8_t i = 0; i < packetLength + 10 ; i++){
            snprintf(str2, sizeof(" %d ")," %d ", packetDestination[i]);
            HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t *> (str2), sizeof(str2), 100);
        }
        snprintf(str3, sizeof(" Packet Length: %d ")," Packet Length: %d ", packetLength);
        HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t *> (str3), sizeof(str3), 100);

        HAL_Delay(1000);
    }

}
