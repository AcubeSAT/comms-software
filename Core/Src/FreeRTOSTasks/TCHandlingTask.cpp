#include "TCHandlingTask.hpp"

extern UART_HandleTypeDef huart3;
bool half_buffer = false;
bool full_buffer = false;

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    half_buffer = true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    full_buffer = true;
}

void TCHandlingTask::execute() {

    while(true){
        if(half_buffer){
            // TC command in upper half of dma buffer [0...tc_command_size]
            half_buffer = false;
            std::copy(rx_dma_buffer, rx_dma_buffer+tc_command_size, tc_command);
            // LOG_DEBUG << "TC_COMMAND[0]";
            etl::string <tc_command_size> cobsDecodedMsg = COBSdecode<tc_command_size>(tc_command, tc_command_size);
            uint8_t messageLength = cobsDecodedMsg.size();
            uint8_t *ecssTCBytes = reinterpret_cast<uint8_t *>(cobsDecodedMsg.data());
            auto ecssTC = MessageParser::parse(ecssTCBytes, messageLength);

            LOG_DEBUG << "Received new TC[" << ecssTC.serviceType << "," << ecssTC.messageType << "]";
            MessageParser::execute(ecssTC);

        }
        if(full_buffer){
            // TC command in upper half of dma buffer [tc_command_size...2*tc_command_size]
            full_buffer = false;
            std::copy(rx_dma_buffer+tc_command_size, rx_dma_buffer+(2*tc_command_size), tc_command);
            // LOG_DEBUG << "TC_COMMAND[1]";
            etl::string<tc_command_size> cobsDecodedMsg = COBSdecode<tc_command_size>(tc_command, tc_command_size);
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

    HAL_UART_Receive_DMA (&huart3, rx_dma_buffer, dma_buffer_size);

    xTaskCreateStatic(vClassTask<TCHandlingTask>,
            this->TaskName,
            TCHandlingTask::TaskStackDepth,
            this,
            tskIDLE_PRIORITY + 5,
            this->taskStack,
            &(this->taskBuffer));

}

