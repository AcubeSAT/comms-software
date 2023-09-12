#pragma once
#include "Task.hpp"
#include "COBS.hpp"

class TCHandlingTask : public Task {
private:
    const uint16_t delayMs = 1000;
    const static inline uint16_t TaskStackDepth = 1000;
    StackType_t taskStack[TaskStackDepth];

    etl::vector<uint8_t, DmaBufferSize> RxDmaBuffer;
    etl::vector<uint8_t, TcCommandSize> TcCommand;

public:

    void execute();

    TCHandlingTask() : Task("TCHandlingTask"){}

    void createTask();

};

inline std::optional<TCHandlingTask> tcHandlingTask;