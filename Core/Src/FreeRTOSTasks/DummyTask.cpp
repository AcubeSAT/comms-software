#include "DummyTask.hpp"
#include "Task.hpp"

void DummyTask::execute() {
    while (true) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        LOG_DEBUG << "Interrupt Count: " << interruptCount;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}