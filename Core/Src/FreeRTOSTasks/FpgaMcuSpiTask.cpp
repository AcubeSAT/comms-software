#include "FpgaMcuSpiTask.hpp"

void FpgaMcuSpiTask::execute() {
    const uint16_t timeoutMs = 1000;


    while (true) {
        // Blocking mode spi and just transmit (simplest scenario)
        const uint16_t outBuffSize = 4;
        const uint8_t outBuff[outBuffSize] = {10, 20, 30, 40};
        HAL_GPIO_WritePin(SPI_NSS_GPIO_Port, SPI_NSS_Pin, GPIO_PIN_RESET); // select slave
        auto status = HAL_SPI_Transmit(&hspi1, outBuff, outBuffSize, timeoutMs); // transmit data
        HAL_GPIO_WritePin(SPI_NSS_GPIO_Port, SPI_NSS_Pin, GPIO_PIN_SET);

        if (status == HAL_OK) {
            LOG_DEBUG << "[FpgaMcuSpiTask] Transmitted data to slave";
        } else {
            LOG_DEBUG << "[FpgaMcuSpiTask] Failed to transmit data to slave";
        }

        // Blocking mode and transmit - receive (more complex, what we will actually need) WIP
        // const uint16_t inBuffSize = 4;
        // uint8_t inBuff[inBuffSize] = {0};
        // etl::string<10*inBuffSize> inBuffString;
        // HAL_GPIO_WritePin(SPI_NSS_GPIO_Port, SPI_NSS_Pin, GPIO_PIN_RESET); // select slave
        // HAL_SPI_TransmitReceive(&hspi1, outBuff, inBuff, outBuffSize + inBuffSize, timeoutMs);
        // HAL_GPIO_WritePin(SPI_NSS_GPIO_Port, SPI_NSS_Pin, GPIO_PIN_SET);
        //
        // if (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
        //     LOG_DEBUG << "[FpgaMcuSpiTask] Failed to transmit-receive data to slave";
        // } else {
        //     for (uint8_t i = 0; i < inBuffSize; i++) {
        //         inBuffString.push_back(inBuff[i]);
        //         inBuffString.push_back(' ');
        //     }
        //     LOG_DEBUG << "[FpgaMcuSpiTask] Transmit-receive complete. Data received: " << inBuffString.data();
        // }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}