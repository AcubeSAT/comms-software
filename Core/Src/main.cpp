#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "InitializationTask.hpp"

extern "C" void main_cpp(){

    initializationTask.emplace();
    initializationTask->createTask();
    vTaskStartScheduler();

    for(;;);
    return;
}

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
extern "C" void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);

    TransceiverTask::transceiver.handle_irq();
}
