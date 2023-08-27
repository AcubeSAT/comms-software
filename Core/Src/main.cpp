#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "DummyTask.h"
#include "MCUTemperatureTask.hpp"
#include "txUHFTask.hpp"
#include "UARTGatekeeperTask.hpp"
#include "TemperatureSensorsTask.hpp"
#include "TransceiverTask.hpp"

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

//namespace AT86RF215 {
//    AT86RF215 transceiver = AT86RF215(&hspi1, AT86RF215Configuration());
//}

extern "C" void main_cpp(){
    uartGatekeeperTask.emplace();
//    mcuTemperatureTask.emplace();
//    temperatureSensorsTask.emplace();
    transceiverTask.emplace();

    uartGatekeeperTask->createTask();
//    temperatureSensorsTask->createTask();
//    mcuTemperatureTask->createTask();
    transceiverTask->createTask();

    vTaskStartScheduler();

//    AT86RF215::Error error;
//
//    AT86RF215::transceiver.chip_reset(error);
//    AT86RF215::transceiver.setup(error);
//    volatile uint16_t  b = AT86RF215::transceiver.get_version_number(error);
//
//    uint8_t irq = AT86RF215::transceiver.get_irq(AT86RF215::RF09, error);
//    uint16_t sum = 0;
//    uint8_t packet[] = {0, 1, 2, 3, 43, 56, 2, 78, 12, 34, 82, 39, 0, 1, 2, 3, 43, 56, 2, 78, 12, 34, 82, 39, 0, 1, 2, 3, 43, 56, 2, 78, 12, 34, 82, 39, 0, 1, 2, 3, 43, 56, 2, 78, 12, 34, 82, 39, 0, 1, 2, 3, 43, 56, 2, 78, 12, 34, 82, 39, 0, 1, 2, 3, 43, 56, 2, 78, 12, 34, 82, 39};
//
//    while(1){
//        volatile AT86RF215::State state = AT86RF215::transceiver.get_state(AT86RF215::RF09, error);
//        state;
//        AT86RF215::transceiver.transmitBasebandPacketsTx(AT86RF215::RF09, packet, 72, error);
//        HAL_Delay(500);
//    }

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

    TransceiverTask::transceiver.handle_irq();
}
