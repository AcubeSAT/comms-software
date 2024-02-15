#include "InitializationTask.hpp"

void InitializationTask::execute() {

    uartGatekeeperTask.emplace();
    dummyTask.emplace();
    temperatureSensorsTask.emplace();
    transceiverTask.emplace();
    watchdogTask.emplace();
    mcuTemperatureTask.emplace();
    timeKeepingTask.emplace();
    currentSensorsTask.emplace();

    uartGatekeeperTask->createTask();
    dummyTask->createTask();
    temperatureSensorsTask->createTask();
    transceiverTask->createTask();
    watchdogTask->createTask();
    mcuTemperatureTask->createTask();
    timeKeepingTask->createTask();
    currentSensorsTask->createTask();

    vTaskSuspend(NULL);
}
