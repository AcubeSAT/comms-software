#include "UARTGatekeeperTask.hpp"

UARTGatekeeperTask::UARTGatekeeperTask() : Task("UARTGatekeeperTask") {
    static StaticQueue_t xStaticQueue;
    uint8_t ucQueueStorageArea[ UARTQueueSize * sizeof(etl::string<LOGGER_MAX_MESSAGE_SIZE>) ];
    xUartQueue = xQueueCreateStatic(UARTQueueSize, sizeof(etl::string<LOGGER_MAX_MESSAGE_SIZE>), ucQueueStorageArea, &xStaticQueue);
}

void UARTGatekeeperTask::execute() {
    UART_HandleTypeDef huart2;
    etl::string<LOGGER_MAX_MESSAGE_SIZE> output;
    while (true) {
        xQueueReceive(xUartQueue, &output, portMAX_DELAY);
        HAL_UART_Transmit(&huart2, reinterpret_cast<const uint8_t *>(output.data()), output.size(), 100);
    }
}