#include "InitializationTask.hpp"

void InitializationTask::execute() {
    uartGatekeeperTask.emplace();
    uartGatekeeperTask->createTask();

    watchdogTask.emplace();
    watchdogTask->createTask();

    timeKeepingTask.emplace();
    timeKeepingTask->createTask();

    dummyTask.emplace();
    dummyTask->createTask();

    // temperatureSensorsTask.emplace();
    // temperatureSensorsTask->createTask();

    // currentSensorsTask.emplace();
    // currentSensorsTask->createTask();

    mcuTemperatureTask.emplace();
    mcuTemperatureTask->createTask();

    // transceiverTask.emplace();
    // transceiverTask->createTask();

#ifdef SERVICE_PARAMETERSTATISTICS
    statisticsReportingTask.emplace();
    statisticsReportingTask->createTask();
#endif

#ifdef SERVICE_TIMESCHEDULING
    timeBasedSchedulingTask.emplace();
    timeBasedSchedulingTask->createTask();
#endif

    vTaskDelete(NULL);
}
