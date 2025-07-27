#pragma once

#include <optional>
#include "Task.hpp"

extern SPI_HandleTypeDef hspi1;
class FpgaMcuSpiTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];
public:
    void execute();

    FpgaMcuSpiTask() : Task("FpgaMcuSpi") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<FpgaMcuSpiTask>, this->TaskName,
                          FpgaMcuSpiTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<FpgaMcuSpiTask> fpgaMcuSpiTask;