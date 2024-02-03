#include "InitializationTask.hpp"

void InitializationTask::execute() {

    dummyTask.emplace();
    temperatureSensorsTask.emplace();
    transceiverTask.emplace();
//    txUHFTask.emplace();
    watchdogTask.emplace();
//    uartGatekeeperTask.emplace();
//    mcuTemperatureTask.emplace();
//    timeKeepingTask.emplace();

    dummyTask->createTask();
    temperatureSensorsTask->createTask();
    transceiverTask->createTask();
//    txUHFTask->createTask();
    watchdogTask->createTask();
//    uartGatekeeperTask->createTask();
//    mcuTemperatureTask->createTask();
//    timeKeepingTask->createTask();

    vTaskSuspend(NULL);
}
