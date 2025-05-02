#pragma once

#include <optional>
#include "Task.hpp"

class CWBeaconTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];
public:
    void execute();

    CWBeaconTask() : Task("CwBeacon") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<CWBeaconTask>, this->TaskName,
                          CWBeaconTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<CWBeaconTask> cwBeaconTask;