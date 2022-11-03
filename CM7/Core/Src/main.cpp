#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOSTasks/DummyTask.h"
#include "CCSDSChannel.hpp"
#include "CCSDSServiceChannel.hpp"
#include "TmTxDataLinkTask.hpp"
#include "DummyPacketMakerTask.hpp"

#include <iostream>

etl::unique_ptr<ServiceChannel> serviceChannelptr;
template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}

void vTask1(void * pvParameters) {
    char count1 = 0;
    for(;;)
    {
        count1++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void * pvParameters) {
    char count2 = 0;
    for(;;)
    {
        count2++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void initiallizeChannels(){
    PhysicalChannel physicalChannel = PhysicalChannel(1024, true,
                                                      12, 1024, 220000, 20);

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
