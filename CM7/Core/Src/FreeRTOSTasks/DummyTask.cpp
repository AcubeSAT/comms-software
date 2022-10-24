#include "FreeRTOSTasks/DummyTask.h"
#include <iostream>
#include "stm32h7xx_hal.h"

void DummyTask::execute() {

    for(;;){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(100);
    }

}