#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "DummyTask.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "MCUTemperatureTask.hpp"
#include "txUHFTask.hpp"
#include "UARTGatekeeperTask.hpp"
#include "TemperatureSensorsTask.hpp"
#include "CurrentSensorsTask.hpp"


extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart3;
extern I2C_HandleTypeDef hi2c2;


template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}

void blinkyTask1(void * pvParameters){
    for(;;){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(50);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        HAL_Delay(50);
    }
}

void blinkyTask2(void * pvParameters){
    for(;;){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(300);
    }
}

void i2cTask(void * parameters) {
    while (true) {
        if (HAL_I2C_Master_Transmit(&hi2c2, 0x40 << 1, NULL, 0, 1000) != HAL_OK) {
            LOG_ERROR << "Bro why\r\n";
        }

        // Check for ACK bit
        if (HAL_I2C_GetError(&hi2c2) == HAL_I2C_ERROR_NONE) {
            LOG_DEBUG << "Success\r\n";
        } else {
            LOG_ERROR << "Nope\r\n";
        }
        vTaskDelay(1000);
    }
}

void i2cRead(void * parameters) {
    uint16_t ina3221Address = 0x40; // INA3221 address
    uint8_t regAddress = 0x03; // shunt voltage 2

    while (true) {
        uint8_t readData[2];

        HAL_I2C_Master_Transmit(&hi2c2, ina3221Address << 1, &regAddress, 1, 1000);

        HAL_I2C_Master_Receive(&hi2c2, ina3221Address << 1, readData, 2, 1000);

        uint16_t received = (readData[0] << 8) | readData[1];
        LOG_DEBUG << "Receiving " << received << "\r\n";
        vTaskDelay(1000);
    }
}

void i2cWriteRead(void * parameters) {
    uint16_t ina3221Address = 0x40; // INA3221 address
    uint8_t regAddress = 0x00; // config register
    uint8_t otherAddress = 0x05; // shunt voltage 2

    uint8_t reset[3] = {regAddress, 0x80, 0x00};
    uint8_t sendData[3] = {regAddress, 0x76, 0x3F};
    uint8_t readData[2];
    LOG_DEBUG << "bro what";

    while (HAL_I2C_IsDeviceReady(&hi2c2, ina3221Address << 1, 0x1000, HAL_MAX_DELAY) != HAL_OK) {
        LOG_DEBUG << "Not ready yet";
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
    // write
    auto error = HAL_I2C_Master_Transmit(&hi2c2, ina3221Address << 1, reset, 3, 1000);
    if (error != HAL_OK) {
        LOG_ERROR << error << "\b\r";
    }

    error = HAL_I2C_Master_Transmit(&hi2c2, ina3221Address << 1, sendData, 3, 1000);

    if (error != HAL_OK) {
        LOG_ERROR << error << "\b\r";
    }

    LOG_DEBUG << "got here \r\n";
    vTaskDelay(pdMS_TO_TICKS(1000));
    // send reg address
    HAL_I2C_Master_Transmit(&hi2c2, ina3221Address << 1, &regAddress, 1, HAL_MAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(1000));
    // read
    HAL_I2C_Master_Receive(&hi2c2, ina3221Address << 1, readData, 2, HAL_MAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(1000));
    uint16_t sent = (sendData[1] << 8) | sendData[2];
    uint16_t received = static_cast<uint16_t>(readData[0] << 8) | static_cast<uint16_t>(readData[1]);

    LOG_DEBUG << "Sending " << sent << " Receiving " << received << "\r\n";
    while (true) {
//        uint8_t readData[2];

        HAL_I2C_Master_Transmit(&hi2c2, ina3221Address << 1, &regAddress, 1, 1000);

        HAL_I2C_Master_Receive(&hi2c2, ina3221Address << 1, readData, 2, 1000);
        LOG_DEBUG << "Current config " << (readData[0] << 8 | readData[1]) << "\r\b";

        HAL_I2C_Master_Transmit(&hi2c2, ina3221Address << 1, &otherAddress, 1, 1000);

        HAL_I2C_Master_Receive(&hi2c2, ina3221Address << 1, readData, 2, 1000);

        uint16_t received = (readData[0] << 8) | readData[1];
        LOG_DEBUG << "Receiving " << received << "\r\n";
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

namespace AT86RF215 {
    AT86RF215 transceiver = AT86RF215(&hspi1, AT86RF215Configuration());
}

extern "C" void main_cpp(){
    uartGatekeeperTask.emplace();
//    mcuTemperatureTask.emplace();
//    temperatureSensorsTask.emplace();
//    currentSensorsTask.emplace();
    uartGatekeeperTask->createTask();
//    currentSensorsTask->createTask();
    xTaskCreate(i2cWriteRead, "i2c verify", 2000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    temperatureSensorsTask->createTask();
//    mcuTemperatureTask->createTask();

    vTaskStartScheduler();

    /**
     * Uncomment below and comment above for Led task visualization (for STM32H743)
     */
//    xTaskCreate(blinkyTask1, "blinkyTask 2", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    xTaskCreate(blinkyTask2, "blinkyTask 2", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
    for(;;);
    return;
}

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
extern "C" void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);

    AT86RF215::transceiver.handle_irq();
}
