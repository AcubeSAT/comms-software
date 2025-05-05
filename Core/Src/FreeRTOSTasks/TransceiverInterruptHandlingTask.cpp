#include "TransceiverInterruptHandlingTask.hpp"
#include "at86rf215.hpp"
extern SPI_HandleTypeDef hspi1;
void TransceiverInterruptHandlingTask::execute() {
    // also initialize the transceiver's resources
    AT86RF215::Error error;
    AT86RF215::transceiverUtils.initializeResources(&hspi1, error);
    if (error != AT86RF215::Error::NO_ERRORS) {
        LOG_ERROR << "[TransceiverInterruptHandlingTask] Failed to initialize transceiver resources";
    }

    while (true) {
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        AT86RF215::Error err;
        AT86RF215::transceiverUtils.handle_irq(err);
    }
    LOG_DEBUG << "[TransceiverInterruptHandlingTask] Interrupt Count: " << interruptCount;
}
