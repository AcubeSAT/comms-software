#include "TCHandlingTask.hpp"

TCHandlingTask::TCHandlingTask() : Task("tcHandlingTask") {
    xQueue = xQueueCreateStatic(queue_size, sizeof(etl::vector<uint8_t, TcCommandSize>),
                       outgoingQueueStorageArea,
                       &outgoingQueueBuffer);
}

void TCHandlingTask::execute() {
    /**
     * The vector that holds the data of the UART message
     * Basically we will hold the data of RxDmaBuffer
    */
    etl::vector<uint8_t, TcCommandSize> TcCommand;
    // start the DMA //
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, RxDmaBuffer.data(), TcCommandSize);
    const TickType_t xTicksToWait = pdMS_TO_TICKS(1000);
    // disabling the half buffer interrupt
    //__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
    // disabling the full buffer interrupt
    //__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_TC);

    for(;;){
        // receive from the Queue //
        if(xQueueReceive(xQueue, TcCommand.data(), xTicksToWait) == pdPASS) {
            for(uint8_t i = 0 ; i < TcCommand.size(); i++)
                LOG_DEBUG << "received : " << i << TcCommand[i] ;
            /*
            // unpacking //
            etl::string<TcCommandSize> cobsDecodedMsg = COBSdecode<TcCommandSize>(TcCommand.data(), TcCommandSize);
            uint8_t messageLength = cobsDecodedMsg.size();
            uint8_t *ecssTCBytes = reinterpret_cast<uint8_t *>(cobsDecodedMsg.data());
            auto ecssTC = MessageParser::parse(ecssTCBytes, messageLength);
            LOG_DEBUG << "Received new TC[" << ecssTC.serviceType << "," << ecssTC.messageType << "]";
            MessageParser::execute(ecssTC);
             */
            // clear the TcCommand Buffer //
            TcCommand.clear();
        }
        else
            LOG_DEBUG << "fail to receive the vector from the queue" ;
        /*
        if(FullBuffer) {
            // TC command in upper half of dma buffer [0...TcCommandSize]
            FullBuffer = false;
            for (uint8_t byte: RxDmaBuffer) {
                if (byte == 0) {
                    break;
                }
                // Copy received command to TcCommand buffer
                TcCommand.push_back(byte);
            }
            new(&(RxDmaBuffer)) uint8_t[TcCommandSize];
            HAL_UART_Receive_DMA(&huart3, RxDmaBuffer, DmaBufferSize);
            HAL_UARTEx_ReceiveToIdle_DMA(&huart3, (uint8_t *)RxDmaBuffer, RxBuf_SIZE);
            // LOG_DEBUG << "TcCommand[0]";
        }
         */
    }
}

