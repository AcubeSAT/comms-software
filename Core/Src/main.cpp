#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "FreeRTOSTasks/DummyTask.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "FreeRTOSTasks/txUHFTask.hpp"
#include "UARTGatekeeperTask.hpp"
#include <iostream>

template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}

//UART_HandleTypeDef huart3;
//SPI_HandleTypeDef hspi1;

void uartTask1(void * pvParameters) {
    char count1 = 0;
    for(;;)
    {
        ++count1;
        etl::string<LOGGER_MAX_MESSAGE_SIZE> output;
        output.append("[%d]Task A running\n\r");
        LOG_DEBUG<<"%d"<< count1 << output.c_str();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartTask2(void * pvParameters) {
    char count2 = 0;
    for(;;)
    {
        UART_HandleTypeDef huart2;
        ++count2;
        etl::string<LOGGER_MAX_MESSAGE_SIZE> output;
        output.append("[%d]Task B running\n\r");
        LOG_DEBUG<<"%d"<< count2 << output.c_str();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void blinkyTask1(void * pvParameters){
    for(;;){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(200);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        HAL_Delay(200);
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

extern "C" void main_cpp(){
    uartGatekeeperTask.emplace();
    uartGatekeeperTask->createTask();
    xTaskCreate(uartTask1, "uartTask 1", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(uartTask2, "uartTask 2", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);

    /**
     * Uncomment below and comment above for Led task visualization (for STM32H743)
     */
    xTaskCreate(blinkyTask1, "blinkyTask 2", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(blinkyTask2, "blinkyTask 2", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
//    txUHFTask.emplace(48000, 4800, false);
//
//    txUHFTask->createTask();
    vTaskStartScheduler();

    for(;;)
    {

    }

    return;
}

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
extern "C" void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);

    AT86RF215::transceiver.handle_irq();
}
