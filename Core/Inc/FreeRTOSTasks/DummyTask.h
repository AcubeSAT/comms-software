#pragma once

#include <optional>
#include "Task.hpp"
#include <etl/optional.h>

class DummyTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];
public:
    void execute();

    DummyTask() : Task("Dummy") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<DummyTask>, this->TaskName,
                          DummyTask::TaskStackDepth, this, tskIDLE_PRIORITY + 2,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline etl::optional<DummyTask> dummyTask;

