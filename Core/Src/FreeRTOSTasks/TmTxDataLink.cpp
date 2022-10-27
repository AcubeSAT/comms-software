#include "TmTxDataLinkTask.hpp"
#include "Task.hpp"
#include "etl/string.h"

extern UART_HandleTypeDef huart3;

void TmTxDataLinkTask::execute(){
    char index = 0;

    uint8_t packet[] = {34,5,76,21,34,2,1,3};
    uint8_t packetDestination[TmTransferFrameSize] ={0};
    for(;;) {
        serviceChannelptr->storePacketTm(packet, 8, 0);
        serviceChannelptr->vcGenerationService(60, 0);
        serviceChannelptr->mcGenerationTMRequest();
        serviceChannelptr->allFramesGenerationTMRequest(packetDestination, TmTransferFrameSize);

        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
        HAL_Delay(100);

        char str[35];
        char byte = packetDestination[index];
        snprintf(str, sizeof("\r\n Transfer Frame data byte %d: %d"),"\r\n Transfer Frame data byte %d: %d", index, byte);
        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(str), sizeof(str), 100);
        index++;

        HAL_Delay(1000);
    }
    
}
