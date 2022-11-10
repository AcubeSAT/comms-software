#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "FreeRTOSTasks/DummyTask.h"
#include "CCSDSChannel.hpp"
#include "CCSDSServiceChannel.hpp"
#include <iostream>
#include "TmTxDataLinkTask.hpp"
#include "DummyPacketMakerTask.hpp"

etl::unique_ptr<ServiceChannel> serviceChannelptr;
template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}

UART_HandleTypeDef huart3;

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
void initiallizeChannels(){
    PhysicalChannel physicalChannel = PhysicalChannel(1024, 12, 1024, 220000, 20);

    etl::flat_map<uint8_t, MAPChannel, MaxMapChannels> mapChannels = {};

    MasterChannel masterChannel = MasterChannel();

    masterChannel.addVC(0, 128, true, 3, 2, true, false, 0,
                        true, SynchronizationFlag::FORWARD_ORDERED,
                        255, 20, 20,
                        10);

    masterChannel.addVC(1, 128, false, 3, 2,
                        true, false, 0,
                        true, SynchronizationFlag::FORWARD_ORDERED,
                        255, 20, 20,
                        10);

    masterChannel.addVC(2, 128, false, 3, 2,
                        true, false, 0,
                        true, SynchronizationFlag::FORWARD_ORDERED,
                        255, 3, 3, 10);

    etl::unique_ptr<ServiceChannel> servChannel(new ServiceChannel(masterChannel, physicalChannel));
    serviceChannelptr = etl::move(servChannel);
}
QueueHandle_t transmitPacketsQueue;
QueueHandle_t transmitPacketLengthsQueue;

extern "C" void main_cpp(){
    //Initiallize Tasks
    initiallizeChannels();

    transmitPacketsQueue = xQueueCreate(200, sizeof(uint8_t));
    transmitPacketLengthsQueue = xQueueCreate(100, sizeof(uint8_t));

    dummyPacketMakerTask.emplace();

    dummyPacketMakerTask->createTask();

    tmTxDataLinkTask.emplace();

    tmTxDataLinkTask->createTask();

    //dummyTask.emplace();

    //dummyTask->createTask();

    vTaskStartScheduler();

    for(;;)

        return;
}
