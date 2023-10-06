#pragma once

#include "Task.hpp"
#include "COBS.hpp"

extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef huart3;

class TCHandlingTask : public Task {
private:
    const uint16_t delayMs = 1000;
    const static inline uint16_t TaskStackDepth = 1000;
    StackType_t taskStack[TaskStackDepth];

    uint8_t RxDmaBuffer[TcCommandSize];
    etl::vector<uint8_t, TcCommandSize> TcCommand;

public:

    void execute();

    TCHandlingTask() : Task("TCHandlingTask") {
        HAL_UART_Receive_DMA(&huart3, RxDmaBuffer, DmaBufferSize);
    }

    void createTask() {
        xTaskCreateStatic(vClassTask < TCHandlingTask > , this->TaskName,
                          TCHandlingTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }

};

inline std::optional<TCHandlingTask> tcHandlingTask;