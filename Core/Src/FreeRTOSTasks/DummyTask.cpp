#include "FreeRTOSTasks/DummyTask.h"
#include "Task.hpp"
#include <iostream>

void DummyTask::execute() {
    for(;;){
        HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
        HAL_Delay(300);
    }
}