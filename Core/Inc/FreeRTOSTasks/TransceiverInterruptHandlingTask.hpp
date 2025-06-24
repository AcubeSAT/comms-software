#pragma once
#include <optional>
#include "Task.hpp"

class TransceiverInterruptHandlingTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];
public:
    [[noreturn]] void execute();
    TaskHandle_t taskHandle;
    uint32_t interruptCount = 0;

    TransceiverInterruptHandlingTask() : Task("TransceiverInterruptHandling") {}

    void createTask(){
        taskHandle = xTaskCreateStatic(vClassTask<TransceiverInterruptHandlingTask>, this->TaskName,
                          TransceiverInterruptHandlingTask::TaskStackDepth, this, tskIDLE_PRIORITY + 3,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<TransceiverInterruptHandlingTask> transceiverInterruptHandlingTask;