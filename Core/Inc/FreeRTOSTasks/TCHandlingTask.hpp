#pragma once

#include "Task.hpp"
#include "COBS.hpp"
#include "queue.h"


extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef huart3;

class TCHandlingTask : public Task {
private:
    // delay in ms //
    const uint16_t delayMs = 1000;
    // stack depth //
    const static inline uint16_t TaskStackDepth = 1000;
    StackType_t taskStack[TaskStackDepth];
    // buffer for handling the data //
    etl::vector<uint8_t, TcCommandSize> TcCommand;
    // buffer that holds the data of the DMA //
    etl::vector<uint8_t, TcCommandSize> RxDmaBuffer;
public:
    // returns the pointer of the pointer that holds the address of the first element of the vector
    // RxDmaPointer
    uint8_t* getRxDmaPointer();
    // define the Queue  //
    QueueHandle_t  xQueue;
    // execute , the while loop //
    void execute();
    // constructor //
    /*
    The HAL_UARTEx_ReceiveToIdle_DMA() function allows to handle reception of Data from Hyperterminal
    using DMA and notify application of already received data while the reception is still ongoing.
    Received characters are handled by DMA and are stored in the user aRXBufferUser buffer.
    Notifications to application, that some data are available in reception buffer, are done
    through the execution of a user callback : HAL_UARTEx_RxEventCallback().
    This callback will be executed when any of following events occurs :
    - HT (Half Transfer) : Half of Rx buffer is filled)
    - TC (Transfer Complete) : Rx buffer is full.
    (In case of Circular DMA, reception could go on, and next reception data will be stored
        in index 0 of reception buffer by DMA).
    - Idle Event on Rx line : Triggered when RX line has been in idle state (normally high state)
    for 1 frame time, after last received byte.
 */
    TCHandlingTask() : Task("TCHandlingTask") {
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, RxDmaBuffer.data(), TcCommandSize);
        // disabling the half buffer interrupt
        __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
        // disabling the full buffer interrupt
        __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_TC);
    }
    // create the task , freeRTOS //
    void createTask() {
        xTaskCreateStatic(vClassTask < TCHandlingTask > , this->TaskName,
                          TCHandlingTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }

};

inline std::optional<TCHandlingTask> tcHandlingTask;