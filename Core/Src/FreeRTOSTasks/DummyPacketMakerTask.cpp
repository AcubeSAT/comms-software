#include "DummyPacketMakerTask.hpp"
#include "stm32h7xx_hal.h"
#include <cstdlib>


void DummyPacketMakerTask::execute() {
    uint8_t randomByte = 0;

    BaseType_t xStatus;
    for(;;) {
        uint8_t randomLength = rand() % 25;
        for (uint8_t i = 0 ; i < randomLength ; i++){
            randomByte = rand() % 256;
            xStatus = xQueueSend(transmitPacketsQueue, &randomByte, pdMS_TO_TICKS(1000));
            if(xStatus == pdFALSE){
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }
        xQueueSend(transmitPacketLengthsQueue, &randomLength, pdMS_TO_TICKS(1000));
    }
}
