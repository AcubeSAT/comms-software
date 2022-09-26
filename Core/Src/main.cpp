#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "FreeRTOSTasks/DummyTask.h"

#include <iostream>

template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}

void vTask1(void * pvParameters) {
    char count1 = 0;
    for(;;)
    {
        count1++;
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
        HAL_Delay(1000);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void * pvParameters) {
    char count2 = 0;
    for(;;)
    {
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
