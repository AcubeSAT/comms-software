#include "TCHandlingTask.hpp"

bool HalfBuffer = false;
bool FullBuffer = false;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    FullBuffer = true;
    HAL_UART_DMAStop(huart);
}

void TCHandlingTask::execute() {

    while (true) {
        if (FullBuffer) {
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

            // LOG_DEBUG << "TcCommand[0]";
            etl::string<TcCommandSize> cobsDecodedMsg = COBSdecode<TcCommandSize>(TcCommand.data(), TcCommandSize);
            uint8_t messageLength = cobsDecodedMsg.size();

            uint8_t *ecssTCBytes = reinterpret_cast<uint8_t *>(cobsDecodedMsg.data());
            auto ecssTC = MessageParser::parse(ecssTCBytes, messageLength);

            LOG_DEBUG << "Received new TC[" << ecssTC.serviceType << "," << ecssTC.messageType << "]";
            MessageParser::execute(ecssTC);

        }
    }
}

