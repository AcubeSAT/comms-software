#include "InitializationTask.hpp"

void InitializationTask::execute() {
    //initializeTasks();
    /*
    uartGatekeeperTask.emplace();
    mcuTemperatureTask.emplace();
    temperatureSensorsTask.emplace();
    timeKeepingTask.emplace();
    watchdogTask.emplace();
    transceiverTask.emplace(); */

    dummyTask->createTask();
    temperatureSensorsTask->createTask();
    transceiverTask->createTask();
    txUHFTask->createTask();
    watchdogTask->createTask();

    vTaskSuspend(NULL);
}
