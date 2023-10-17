#include "TCHandlingTask.hpp"

void TCHandlingTask::execute() {
    // TcCommand : the vector win which we copy the data from the RxDmaBuffer //
    etl::vector<uint8_t, TcCommandSize> TcCommand;
    // Handle of the calling task //
    taskHandle = xTaskGetCurrentTaskHandle();
    // disabling the half buffer interrupt //
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);
    // disabling the full buffer interrupt //
    __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_TC);

    for(;;){
        // Awaiting a notification indicating the arrival of data from the UART //
        xTaskNotifyWait(0, 0, nullptr, portMAX_DELAY);

        // copy the data from UART to the TcCommand vector //
        for(uint8_t i = 0 ; i < tcHandlingTask->Size ; i++)
            TcCommand.push_back(tcHandlingTask->RxDmaBuffer[i]);

        // clear the buffer to receive again new data from the UART interface //
        tcHandlingTask->RxDmaBuffer.clear();

        // HANDLING OF THE UART DATA //
        etl::string<TcCommandSize> cobsDecodedMsg = COBSdecode<TcCommandSize>(TcCommand.data(), tcHandlingTask->Size);
        // decoded_size = Size - 1 //
        uint8_t messageLength = cobsDecodedMsg.size();
        uint8_t *ecssTCBytes = reinterpret_cast<uint8_t *>(cobsDecodedMsg.data());
        auto ecssTC = MessageParser::parse(ecssTCBytes, messageLength);
        LOG_DEBUG << "Received new TC[" << ecssTC.serviceType << "," << ecssTC.messageType << "]";
        MessageParser::execute(ecssTC);

        // clear the TcCommand buffer //
        TcCommand.clear();
    }
}


