#include "FreeRTOSTasks/DummyTask.h"
#include "Task.hpp"
#include <iostream>

void DummyTask::execute() {
    for(;;){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(300);
    }
}