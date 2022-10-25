#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "FreeRTOSTasks/DummyTask.h"
#include "FreeRTOSTasks/txUHFTask.hpp"

#include <iostream>

template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}

extern UART_HandleTypeDef huart3;

void uartTask1(void * pvParameters) {
    char count1 = 0;
    for(;;)
    {
        ++count1;
        char str[30];
        snprintf(str, sizeof("[%d]Task A running\n\r"),"[%d]Task A running\n\r", count1);
        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(str), sizeof(str), 100);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartTask2(void * pvParameters) {
    char count2 = 0;
    for(;;)
    {
        ++count2;
        char str[30];
        snprintf(str, sizeof("[%d]Task B running\n\r"),"[%d]Task B running\n\r", count2);
        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(str), sizeof(str), 100);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void blinkyTask1(void * pvParameters){
    for(;;){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(50);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        HAL_Delay(50);
    }
}

void blinkyTask2(void * pvParameters){
    for(;;){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(300);
    }
}

extern "C" void main_cpp(){

//    xTaskCreate(uartTask1, "uartTask 1", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
//    xTaskCreate(uartTask2, "uartTask 2", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);

    /**
     * Uncomment below and comment above for Led task visualization (for STM32H743)
     */
//    xTaskCreate(blinkyTask1, "blinkyTask 2", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
//    xTaskCreate(blinkyTask2, "blinkyTask 2", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
    txUHFTask.emplace(48000, 4800, false);

    txUHFTask->createTask();
    vTaskStartScheduler();

    for(;;)

    return;
}
