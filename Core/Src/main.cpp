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
#include "TimeKeepingTask.hpp"
#include "TCHandlingTask.hpp"
#include "CAN.hpp"
#include "stm32h7xx_hal_fdcan.h"
#include "CANGatekeeperTask.hpp"
#include "WatchdogTask.hpp"

extern SPI_HandleTypeDef hspi1;

// extern I2C_HandleTypeDef hi2c2;

// extern RTC_HandleTypeDef hrtc;

namespace AT86RF215 {
    AT86RF215 transceiver = AT86RF215(&hspi1, AT86RF215Configuration());
}

extern "C" void main_cpp(){
    uartGatekeeperTask.emplace();
    mcuTemperatureTask.emplace();
    temperatureSensorsTask.emplace();
    timeKeepingTask.emplace();
    tcHandlingTask.emplace();
    watchdogTask.emplace();
    canTestTask.emplace();
    canGatekeeperTask.emplace();

    uartGatekeeperTask->createTask();
    temperatureSensorsTask->createTask();
    mcuTemperatureTask->createTask();
    timeKeepingTask->createTask();
    tcHandlingTask->createTask();
    watchdogTask->createTask();
    canTestTask->createTask();
    canGatekeeperTask->createTask();

    vTaskStartScheduler();

    for(;;);
    return;
}

/**
 * HAL_UARTEx_RxEventCallback is a callback function for UART receiving which is enable by the
 * the call of the function HAL_UARTEx_ReceiveToIdle_DMA().
 * It is triggered whenever the UART line is IDLE.
 * @param huart : pointer of type UART_HandleTypeDef
 * @param Size : incoming size of data in bytes
 */
extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    // Size is used for copying the right size of data to the TcCommand buffer,
    // which belongs to the execute() of the tcHandlingTask
    tcHandlingTask -> incomingMessageSize = Size;
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(tcHandlingTask->taskHandle, 0, eNoAction,  &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    // Reset the DMA to receive the next chunk of data
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, tcHandlingTask->RxDmaBuffer.data(), TcCommandSize);
}

extern "C" void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
   if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
       /* Retrieve Rx messages from RX FIFO0 */
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &CAN::rxHeader0, CAN::rxFifo0.data()) != HAL_OK) {
            /* Reception Error */
            Error_Handler();
        }
        CAN::rxFifo0.repair();
        CAN::Frame newFrame = CAN::getFrame(&CAN::rxFifo0, CAN::rxHeader0.Identifier);
        canGatekeeperTask->addToIncoming(newFrame);

        if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
            /* Notification Error */
            Error_Handler();
        }
    }
}


/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
extern "C" void EXTI15_10_IRQHandler(void){
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    AT86RF215::transceiver.handle_irq();
}