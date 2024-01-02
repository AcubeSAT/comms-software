#pragma once

#include <optional>
#include "Task.hpp"

class PSUEnableMonitorTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];
public:
    void execute();

    PSUEnableMonitorTask() : Task("PSUEnableMonitor") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<PSUEnableMonitorTask>, this->TaskName,
                          PSUEnableMonitorTask::TaskStackDepth, this, tskIDLE_PRIORITY + 5,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<PSUEnableMonitorTask> psuEnableMonitorTask;

