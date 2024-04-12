#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "DummyTask.h"
#include "MCUTemperatureTask.hpp"
#include "UARTGatekeeperTask.hpp"
#include "TemperatureSensorsTask.hpp"
#include "CurrentSensorsTask.hpp"
#include "TransceiverTask.hpp"
#include "TimeKeepingTask.hpp"
#include "WatchdogTask.hpp"
#include "CANGatekeeperTask.hpp"
#include "CANTestTask.hpp"

extern "C" void main_cpp(){
    uartGatekeeperTask.emplace();
    canGatekeeperTask.emplace();
    canTestTask.emplace();
//    mcuTemperatureTask.emplace();
//    temperatureSensorsTask.emplace();
//    timeKeepingTask.emplace();
//    currentSensorsTask.emplace();
//    transceiverTask.emplace();
    watchdogTask.emplace();
    dummyTask.emplace();

    uartGatekeeperTask->createTask();
    canGatekeeperTask->createTask();
    canTestTask->createTask();
//    mcuTemperatureTask->createTask();
//    temperatureSensorsTask->createTask();
//    timeKeepingTask->createTask();
//    currentSensorsTask->createTask();
//    transceiverTask->createTask();
    dummyTask->createTask();
    watchdogTask->createTask();

    vTaskStartScheduler();

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

extern "C" void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
        /* Retreive Rx messages from RX FIFO0 */

        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &CAN::rxHeader0, CAN::rxFifo0.data()) != HAL_OK) {
            /* Reception Error */
            Error_Handler();
        }
//        canGatekeeperTask->switchActiveBus(CAN::Main);
        CAN::rxFifo0.repair();
        CAN::Frame newFrame = CAN::getFrame(&CAN::rxFifo0, CAN::rxHeader0.Identifier);
        if(CAN::rxFifo0[0] >> 6 == CAN::TPProtocol::Frame::Single){
            canGatekeeperTask->addSFToIncoming(newFrame);
            xTaskNotifyFromISR(canGatekeeperTask->taskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);

        }else{
            canGatekeeperTask->addMFToIncoming(newFrame);
            if(CAN::rxFifo0[0] >> 6 == CAN::TPProtocol::Frame::Final){
                xTaskNotifyFromISR(canGatekeeperTask->taskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }

        if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
            /* Notification Error */
            Error_Handler();
        }
    }
}

extern "C" void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != RESET) {
        /* Retreive Rx messages from RX FIFO1 */
        if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &CAN::rxHeader1, CAN::rxFifo1.data()) != HAL_OK) {
            /* Reception Error */
            Error_Handler();
        }
//        canGatekeeperTask->switchActiveBus(CAN::Redundant);
        CAN::rxFifo1.repair();
        CAN::Frame newFrame = CAN::getFrame(&CAN::rxFifo1, CAN::rxHeader1.Identifier);
        if(CAN::rxFifo0[0] >> 6 == CAN::TPProtocol::Frame::Single){
            canGatekeeperTask->addSFToIncoming(newFrame);
            xTaskNotifyFromISR(canGatekeeperTask->taskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);

        }else{
            canGatekeeperTask->addMFToIncoming(newFrame);
            if(CAN::rxFifo0[0] >> 6 == CAN::TPProtocol::Frame::Final){
                xTaskNotifyFromISR(canGatekeeperTask->taskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }

        if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK) {
            /* Notification Error */
            Error_Handler();
        }
    }
}
