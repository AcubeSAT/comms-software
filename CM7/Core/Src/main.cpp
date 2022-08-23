#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "FreeRTOSTasks/DummyTask.h"

#include <iostream>

//UART_HandleTypeDef huart3;

template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}

void vTask1(void * pvParameters) {
    char str1[100] = "";
    char count1 = 0;
    for(;;)
    {
//        sprintf(reinterpret_cast<char *>(str1), "Task 1 Running\n\r");
//        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(str1), sizeof(str1), 100);
        count1++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void * pvParameters) {
    char str2[100] = "";
    char count2 = 0;
    for(;;)
    {
//        sprintf(reinterpret_cast<char *>(str2), "Task 4 Running\n\r");
//        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(str2), sizeof(str2), 100);
        count2++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void main_cpp(){

    xTaskCreate(vTask1, "Task 1", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vTask2, "Task 2", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
    vTaskStartScheduler();

    for(;;)

    return;
}
