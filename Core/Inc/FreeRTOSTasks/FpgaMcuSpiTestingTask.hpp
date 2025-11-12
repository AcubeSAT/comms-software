#pragma once

#include <optional>
#include "Task.hpp"

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
class FpgaMcuSpiTestingTask : public Task {
private:
    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];
public:
    void execute();

    FpgaMcuSpiTestingTask() : Task("FpgaMcuSpi") {}

    void createTask(){
        xTaskCreateStatic(vClassTask<FpgaMcuSpiTestingTask>, this->TaskName,
                          FpgaMcuSpiTestingTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<FpgaMcuSpiTestingTask> fpgaMcuSpiTestingTask;