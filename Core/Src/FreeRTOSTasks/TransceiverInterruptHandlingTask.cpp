#include "TransceiverInterruptHandlingTask.hpp"
#include "at86rf215.hpp"

void TransceiverInterruptHandlingTask::execute() {
    // // Now that the task is running anc can receive notifications, enable interrupts
    // HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    // not sure why, but artificially triggering an interrupt in the start
    // is necessary, otherwise no interrupts can occur
    __HAL_GPIO_EXTI_GENERATE_SWIT(RF_IRQ_Pin);

    while (true) {
        xTaskNotifyWait(0, 0, &interruptCount, portMAX_DELAY);
        AT86RF215::Error err;
        AT86RF215::transceiverUtils.handle_irq(err);
        LOG_DEBUG << "[TransceiverInterruptHandlingTask] Interrupt Count: " << interruptCount;
    }
}

