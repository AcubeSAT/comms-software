#include "FreeRTOSTasks/DummyTask.h"
#include <iostream>
#include "stm32h7xx_hal.h"

//void DummyTask::execute() {
//    uint8_t counter = 0;
//
//    for (int i=0; i<100; i++) {
//        char str[100];
//        UART_HandleTypeDef huart3;
//        uint32_t e = 1;
//        sprintf(str ,"Time needed to modulate 200 kbits %d ms\r\n", counter);
//        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(str), sizeof(str), 100);
//        vTaskDelay(pdMS_TO_TICKS(10));
//        counter++;
//    }
//
//}