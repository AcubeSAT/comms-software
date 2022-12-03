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
            etl::string<40> errMessage = "Error in storePacket Function";
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errMessage.data()), errMessage.size(), 100);
        }
        err = serviceChannelptr->vcGenerationService(60, 0);
        if(err == MEMORY_POOL_FULL){
            etl::string<40> errMessage = "Memory Pool Fool";
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errMessage.data()), errMessage.size(), 100);
        }
        err = serviceChannelptr->mcGenerationTMRequest();
        if(err == NO_RX_PACKETS_TO_PROCESS){
            etl::string<40> errMessage = "Empty buffer in MC Generation";
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errMessage.data()), errMessage.size(), 100);
        }
        else if(err == TX_MC_FRAME_BUFFER_FULL){
            etl::string<40> errMessage = "Buffer full in MC Generation";
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errMessage.data()), errMessage.size(), 100);
        }
        err = serviceChannelptr->allFramesGenerationTMRequest(packetDestination, TmTransferFrameSize);
        if(err != NO_SERVICE_EVENT){
            etl::string<40> errMessage = "Error in All Frames Generation Service";
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(errMessage.data()), errMessage.size(), 100);
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
        HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t *>(string.data()), string.size(), 100);
        HAL_Delay(1000);
    }
}
