#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "DummyTask.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "txUHFTask.hpp"
#include "UARTGatekeeperTask.hpp"
#include "TMP117.hpp"

extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c2;


template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}


void uartTask1(void * pvParameters) {
    for(;;)
    {
        etl::string<30> str = "[%d]Task A running\r\n";
        uartGatekeeperTask->addToQueue(str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartTask2(void * pvParameters) {
    for(;;)
    {
        etl::string<30> str = "[%d]Task B running\r\n";
        uartGatekeeperTask->addToQueue(str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
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
namespace AT86RF215 {
    AT86RF215 transceiver = AT86RF215(&hspi1, AT86RF215Configuration());
}

void tempTask(void * pvParameters){
    auto config = TMP117::Config();
    TMP117::TMP117 tempSensor = TMP117::TMP117(hi2c2, TMP117::I2CAddress::ADDRESS_1, config);
    etl::format_spec format;
    for(;;){
        std::pair<TMP117::Error, uint16_t> temp = tempSensor.getTemperature(true);
        etl::string<50> str = "Temperature is ";
        etl::string<30> value;
        if (temp.first == TMP117::Error::NO_ERRORS){
            etl::to_string(tempSensor.convertTemperature(temp.second), value, format);
        }
        else{
            value = "{Error getting temperature}\r\n";
        }
        str.append(value);
        uartGatekeeperTask->addToQueue(str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

extern "C" void main_cpp(){
    uartGatekeeperTask.emplace();
    xTaskCreate(tempTask, "tempTask", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    xTaskCreate(uartTask2, "uartTask 2", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    txUHFTask.emplace(48000, 4800, false);
//    txUHFTask->createTask();
    uartGatekeeperTask->createTask();
//    auto output = String<ECSSMaxMessageSize>("New ");
//    LOG_DEBUG<<output.c_str();
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
