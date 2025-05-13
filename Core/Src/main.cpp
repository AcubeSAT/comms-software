#include "main.h"

#include <CWBeaconTask.hpp>

#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "DummyTask.h"
#include "at86rf215.hpp"
#include "MCUTemperatureTask.hpp"
#include "UARTGatekeeperTask.hpp"
#include "TemperatureSensorsTask.hpp"
#include "CurrentSensorsTask.hpp"
#include "TimeKeepingTask.hpp"
#include "WatchdogTask.hpp"
#include "TransceiverBasebandCoreTask.hpp"
#include "TransceiverInterruptHandlingTask.hpp"

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart3;
extern I2C_HandleTypeDef hi2c2;
extern RTC_HandleTypeDef hrtc;

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

extern "C" void main_cpp(){
    /** FreeRTOS Tasks **/
    uartGatekeeperTask.emplace();
    //mcuTemperatureTask.emplace();
    //temperatureSensorsTask.emplace();
    //timeKeepingTask.emplace();
    //currentSensorsTask.emplace();
    transceiverInterruptHandlingTask.emplace();
    //transceiverBasebandCoreTask.emplace();
    cwBeaconTask.emplace();
    watchdogTask.emplace();

    uartGatekeeperTask->createTask();
    //mcuTemperatureTask->createTask();
    //temperatureSensorsTask->createTask();
    //timeKeepingTask->createTask();
    //currentSensorsTask->createTask();
    transceiverInterruptHandlingTask->createTask();
    //transceiverBasebandCoreTask->createTask();
    cwBeaconTask->createTask();
    watchdogTask->createTask();

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
 * @brief This function handles EXTI15_10 line interrupts
 * @note The transceiver interrupt pin is assigned to this line (extremely time critical application).
 */
extern "C" void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(RF_IRQ_Pin);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(transceiverInterruptHandlingTask->taskHandle, 0, eIncrement, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* SPI callbacks in non blocking mode (DMA)*/
extern "C" [[maybe_unused]] void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (hspi == &hspi1) {
        xEventGroupSetBitsFromISR(AT86RF215::transceiverUtils.eventGroupHandle,
            AT86RF215::spiWriteCompleteGroupBit,
            &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

extern "C" [[maybe_unused]] void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (hspi == &hspi1) {
        xEventGroupSetBitsFromISR(AT86RF215::transceiverUtils.eventGroupHandle,
            AT86RF215::spiReadCompleteGroupBit,
            &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


