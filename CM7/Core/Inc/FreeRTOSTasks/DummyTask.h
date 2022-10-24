#pragma once

#include <optional>
#include "Task.hpp"

class DummyTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];
public:
    void execute();

    DummyTask() : Task("Dummy") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<DummyTask>, this->TaskName,
                          DummyTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<DummyTask> dummyTask;

