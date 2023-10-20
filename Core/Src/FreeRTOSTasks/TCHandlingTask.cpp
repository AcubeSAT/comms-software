#include "TCHandlingTask.hpp"

void TCHandlingTask::execute() {
    taskHandle = xTaskGetCurrentTaskHandle();
    while(1){
        // Awaiting a notification indicating the arrival of data from the UART //
        xTaskNotifyWait(0, 0, nullptr, portMAX_DELAY);

        for(uint8_t i = 0 ; i < tcHandlingTask->incomingMessageSize; i++)
            TcCommand.push_back(tcHandlingTask->RxDmaBuffer[i]);
        // Clear the RxDmaBuffer to receive the new data //
        tcHandlingTask->RxDmaBuffer.clear();

        // TC Parsing and Execution
        etl::string<TcCommandSize> cobsDecodedMsg = COBSdecode<TcCommandSize>(TcCommand.data(), tcHandlingTask->incomingMessageSize);
        // decoded_size = incomingMessageSize - 1 //
        uint8_t messageLength = cobsDecodedMsg.size();
        uint8_t *ecssTCBytes = reinterpret_cast<uint8_t *>(cobsDecodedMsg.data());
        auto ecssTC = MessageParser::parse(ecssTCBytes, messageLength);
        LOG_DEBUG << "Received new TC[" << ecssTC.serviceType << "," << ecssTC.messageType << "]";
        MessageParser::execute(ecssTC);

        TcCommand.clear();
    }
}


