#include "TCHandlingTask.hpp"

extern UART_HandleTypeDef huart3;
bool HalfBuffer = false;
bool FullBuffer = false;

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    HalfBuffer = true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    FullBuffer = true;
}

void TCHandlingTask::execute() {

    while(true){
        if(HalfBuffer){
            // TC command in upper half of dma buffer [0...TcCommandSize]
            HalfBuffer = false;
            for(uint8_t i=0;i<TcCommandSize;i++){
                // Copy received command to TcCommand buffer
                TcCommand.push_back(RxDmaBuffer[i]);
            }
            // LOG_DEBUG << "TcCommand[0]";
            etl::string <TcCommandSize> cobsDecodedMsg = COBSdecode<TcCommandSize>(TcCommand.data(), TcCommandSize);
            uint8_t messageLength = cobsDecodedMsg.size();

            uint8_t *ecssTCBytes = reinterpret_cast<uint8_t *>(cobsDecodedMsg.data());
            auto ecssTC = MessageParser::parse(ecssTCBytes, messageLength);

            LOG_DEBUG << "Received new TC[" << ecssTC.serviceType << "," << ecssTC.messageType << "]";
            MessageParser::execute(ecssTC);

        }
        if(FullBuffer){
            // TC command in upper half of dma buffer [TcCommandSize...2*TcCommandSize]
            FullBuffer = false;
            for(uint8_t i=TcCommandSize;i<(2*TcCommandSize);i++){
                TcCommand.push_back(RxDmaBuffer[i]);
            }
            // LOG_DEBUG << "TcCommand[1]";
            etl::string<TcCommandSize> cobsDecodedMsg = COBSdecode<TcCommandSize>(TcCommand.data(), TcCommandSize);
            uint8_t messageLength = cobsDecodedMsg.size();

            uint8_t *ecssTCBytes = reinterpret_cast<uint8_t *>(cobsDecodedMsg.data());
            auto ecssTC = MessageParser::parse(ecssTCBytes, messageLength);

            LOG_DEBUG << "Received new TC[" << ecssTC.serviceType << "," << ecssTC.messageType << "]";
            MessageParser::execute(ecssTC);
        }

        vTaskDelay(pdMS_TO_TICKS(delayMs));
    }

}

void TCHandlingTask::createTask() {

    HAL_UART_Receive_DMA (&huart3, RxDmaBuffer.data(), DmaBufferSize);

    xTaskCreateStatic(vClassTask<TCHandlingTask>,
            this->TaskName,
            TCHandlingTask::TaskStackDepth,
            this,
            tskIDLE_PRIORITY + 5,
            this->taskStack,
            &(this->taskBuffer));

}

