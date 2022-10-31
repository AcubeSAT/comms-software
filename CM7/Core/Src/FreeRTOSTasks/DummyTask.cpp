#include "FreeRTOSTasks/DummyTask.h"
#include <iostream>
#include "stm32h7xx_hal.h"

extern UART_HandleTypeDef huart3;

void DummyTask::execute() {

    for(;;){
        char str[20];
        snprintf(str,sizeof("\rDummy Task Running\n"), "\rDummyTask Running\n");
        HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t*>(str), sizeof(str), 100);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}