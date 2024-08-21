#include "FreeRTOSTasks/DummyTask.h"
#include "Task.hpp"

void DummyTask::execute() {
    for(;;){
        HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}