#pragma once
#include "Task.hpp"
#include "COBS.hpp"

class TCHandlingTask : public Task {
private:
    const uint16_t delayMs = 1000;
    const static inline uint16_t TaskStackDepth = 1000;
    StackType_t taskStack[TaskStackDepth];

    const static uint8_t tc_command_size = 64;
    const static uint16_t dma_buffer_size = 2*tc_command_size;
    uint8_t rx_dma_buffer [dma_buffer_size];
    uint8_t tc_command[tc_command_size];


public:

    void execute();

    TCHandlingTask() : Task("TCHandlingTask"){}

    void createTask();

};

inline std::optional<TCHandlingTask> tcHandlingTask;