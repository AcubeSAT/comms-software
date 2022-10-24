#include "TmTxDataLinkTask.hpp"
#include "Task.hpp"
#include "etl/string.h"

extern UART_HandleTypeDef huart3;

void TmTxDataLinkTask::execute(){
    volatile char byte = 0;
    char str[30];
    uint8_t packet[] = {34,5,76,21,34,2,1,3};
    uint8_t packetDestination[TmTransferFrameSize] ={0};
    for(;;) {
        serviceChannelptr->storePacketTm(packet, 8, 0);
        serviceChannelptr->vcGenerationService(60, 0);
        serviceChannelptr->mcGenerationTMRequest();
        serviceChannelptr->allFramesGenerationTMRequest(packetDestination, TmTransferFrameSize);

        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
        HAL_Delay(100);

        for(uint8_t i = 0 ; i<30 ; i++){
            byte = packetDestination[i];
            snprintf(str, sizeof(" %d", byte), " %d", byte);
            HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t *>(str), sizeof(str), 100);
        }
        //(char *)packetDestination;
        //(char)(kikaByte);
        //snprintf(str, sizeof("[%d]TmTx Data Link Task\n\r"),"[%d]TmTx Data Link Task\n\r", serviceChannelptr->frameAfterALlFramesGenerationService()->packetData()[0]);

        //HAL_UART_Transmit(&huart3,reinterpret_cast<uint8_t *>(kikaByte),sizeof(kikaByte),100);
        HAL_Delay(100);
        //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
        //vTaskDelay(pdMS_TO_TICKS(100));

    }
    
}
