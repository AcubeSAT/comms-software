#include "FreeRTOSTasks/DummyTask.h"
#include "Task.hpp"

void DummyTask::execute() {
    while (true) {
//        LOG_DEBUG << " Hell o comms";
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}