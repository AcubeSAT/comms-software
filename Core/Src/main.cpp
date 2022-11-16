#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "FreeRTOSTasks/DummyTask.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "FreeRTOSTasks/txUHFTask.hpp"

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
        char str[30];
        snprintf(str, sizeof("[%d]Task A running\n\r"),"[%d]Task A running\n\r", count1);
        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(str), sizeof(str), 100);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartTask2(void * pvParameters) {
    char count2 = 0;
    for(;;)
    {
        ++count2;
        char str[30];
        snprintf(str, sizeof("[%d]Task B running\n\r"),"[%d]Task B running\n\r", count2);
        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(str), sizeof(str), 100);
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

extern "C" void main_cpp(){

//    xTaskCreate(uartTask1, "uartTask 1", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
//    xTaskCreate(uartTask2, "uartTask 2", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);

    /**
     * Uncomment below and comment above for Led task visualization (for STM32H743)
     */
//    xTaskCreate(blinkyTask1, "blinkyTask 2", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
//    xTaskCreate(blinkyTask2, "blinkyTask 2", 1000, NULL, tskIDLE_PRIORITY + 1, NULL);
//    dummyTask.emplace();
//
//    dummyTask->createTask();
//    vTaskStartScheduler();

    AT86RF215::Error error;

    AT86RF215::transceiver.chip_reset(error);
    AT86RF215::transceiver.setup(error);

    uint8_t irq = AT86RF215::transceiver.get_irq(AT86RF215::RF09, error);
    uint16_t sum = 0;
    uint8_t packet[] = {0, 1, 2, 3, 43, 56, 2, 78, 12, 34, 82, 39};

    while(1){
        volatile AT86RF215::State state = AT86RF215::transceiver.get_state(AT86RF215::RF09, error);
        state;
        AT86RF215::transceiver.transmitBasebandPacketsTx(AT86RF215::RF09, packet, 12, error);
        HAL_Delay(500);
    }

    for(;;)

    return;
}

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
extern "C" void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);

    AT86RF215::transceiver.handle_irq();
}
