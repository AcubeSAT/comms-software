#include "UARTGatekeeperTask.hpp"

UARTGatekeeperTask::UARTGatekeeperTask() : Task("UARTGatekeeperTask") {
    xUartQueue = xQueueCreateStatic(UARTQueueSize, sizeof(etl::string<LOGGER_MAX_MESSAGE_SIZE>), ucQueueStorageArea, &xStaticQueue);
}

void UARTGatekeeperTask::execute() {
    etl::string<LOGGER_MAX_MESSAGE_SIZE> output;
    while (true) {
        xQueueReceive(xUartQueue, &output, portMAX_DELAY);
        extern UART_HandleTypeDef huart3;
        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(output.data()), output.size(), 100);
    }
}
