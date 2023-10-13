#include "TCHandlingTask.hpp"



void TCHandlingTask::execute() {
    /**
     * The vector that holds the data of the UART message
     * Basically we will hold the data of RxDmaBuffer
    */
    etl::vector<uint8_t, TcCommandSize> TcCommand;

    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, RxDmaBuffer.data(), TcCommandSize);
    // disabling the half buffer interrupt
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
    // disabling the full buffer interrupt
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_TC);

    for(;;){
        // receive from the Queue //
        xQueueReceive(xQueue, &TcCommand, portMAX_DELAY) ;

        etl::string<TcCommandSize> cobsDecodedMsg = COBSdecode<TcCommandSize>(TcCommand.data(), TcCommandSize);
        TcCommand.clear();
        uint8_t messageLength = cobsDecodedMsg.size();
        uint8_t *ecssTCBytes = reinterpret_cast<uint8_t *>(cobsDecodedMsg.data());
        auto ecssTC = MessageParser::parse(ecssTCBytes, messageLength);
        LOG_DEBUG << "Received new TC[" << ecssTC.serviceType << "," << ecssTC.messageType << "]";
        MessageParser::execute(ecssTC);
    }
}


