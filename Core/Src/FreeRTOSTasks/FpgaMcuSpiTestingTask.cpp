#include "FpgaMcuSpiTestingTask.hpp"

void FpgaMcuSpiTestingTask::execute() {
    const uint16_t timeoutMs = 3000;
    constexpr bool onlyTransmit = true;

    while (true) {
        const uint16_t size = 8;
        const uint8_t outBuff[size] = {'G', 'k', 'a', 'r', 'i', 'p', 'i', 's'};
        uint8_t inBuff[size] = {0};
        etl::string<size> inBuffString;

        if constexpr (onlyTransmit) {
            // Blocking mode spi and just transmit (simplest scenario)
            HAL_GPIO_WritePin(RF_NSS_GPIO_Port, RF_NSS_Pin, GPIO_PIN_RESET); // select slave
            auto status = HAL_SPI_Transmit(&hspi1, outBuff, size, timeoutMs); // transmit data
            HAL_GPIO_WritePin(RF_NSS_GPIO_Port, RF_NSS_Pin, GPIO_PIN_SET);

            if (status == HAL_OK) {
                LOG_DEBUG << "[FpgaMcuSpiTestingTask] Transmitted data to slave";
            }
            else {
                LOG_DEBUG << "[FpgaMcuSpiTestingTask] Failed to transmit data to slave";
            }
        } else {
            // Blocking mode and transmit - receive (more complex, what we will actually need)
            HAL_GPIO_WritePin(RF_NSS_GPIO_Port, RF_NSS_Pin, GPIO_PIN_RESET); // select slave
            HAL_SPI_TransmitReceive(&hspi1, outBuff, inBuff, size, timeoutMs);
            HAL_GPIO_WritePin(RF_NSS_GPIO_Port, RF_NSS_Pin, GPIO_PIN_SET);

            if (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY) {
                LOG_DEBUG << "[FpgaMcuSpiTestingTask] Failed to transmit-receive data to slave";
            }
            else {
                for (unsigned char i : inBuff) {
                    inBuffString.push_back(i);
                }
                LOG_DEBUG << "[FpgaMcuSpiTestingTask] Transmit-receive complete.\nData received: " << inBuffString.
                    data();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(timeoutMs));
    }
}
