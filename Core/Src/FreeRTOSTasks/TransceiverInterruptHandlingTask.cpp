#include "TransceiverInterruptHandlingTask.hpp"
#include "at86rf215.hpp"

void TransceiverInterruptHandlingTask::execute() {
    while (true) {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        AT86RF215::Error err;
        AT86RF215::transceiverUtils.handle_irq(err);
    }
    LOG_DEBUG << "[TransceiverInterruptHandlingTask] Interrupt Count: " << interruptCount;
}
