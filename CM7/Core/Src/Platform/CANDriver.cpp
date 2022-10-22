#include "CANDriver.hpp"
#include "Logger.hpp"

uint8_t CANDriver::convertDlcToLength(uint8_t dlc) {
    static constexpr uint8_t msgLength[] = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};
    return msgLength[dlc];
}

uint8_t CANDriver::convertLengthToDLC(uint8_t length) {
    uint8_t dlc;

    if (length <= 8U) {
        dlc = length;
    } else if (length <= 12U) {
        dlc = 0x9U;
    } else if (length <= 16U) {
        dlc = 0xAU;
    } else if (length <= 20U) {
        dlc = 0xBU;
    } else if (length <= 24U) {
        dlc = 0xCU;
    } else if (length <= 32U) {
        dlc = 0xDU;
    } else if (length <= 48U) {
        dlc = 0xEU;
    } else {
        dlc = 0xFU;
    }
    return dlc;
}

void CANDriver::HAL_FDCAN_RxFifoCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifoITs, FDCAN_RxHeaderTypeDef rxFifo)
{
    if (hfdcan->Instance == FDCAN1)
    {
        if ((RxFifoITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
        {
            //HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0); θα μπει στον constructror μια φορά
            logMessage(rxFifo);
        }
    }
}

void CANDriver::HAL_FDCAN_TxFifoCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TxFifoITs)
{
    if (hfdcan->Instance == FDCAN1)
    {
        if ((TxFifoITs & FDCAN_IT_TX_FIFO_EMPTY) != RESET)
        {
           LOG_DEBUG << "SUCCESSFUL TRANSMISSION";
        }
    }
}

void CANDriver::logMessage(const FDCAN_RxHeaderTypeDef &rxBuf) {
    auto message = String<ECSSMaxStringSize>("CAN Message: ");
    uint32_t id = rxBuf.Identifier;
    const uint8_t MsgLength = convertDlcToLength(rxBuf.DataLength);
    message.append("Message - ID : ");
    etl::to_string(id, message, etl::format_spec().hex(), true);
    message.append(" Length : ");
    etl::to_string(MsgLength, message, true);
    message.append(" Message : ");
    for (uint8_t idx = 0; idx < MsgLength; idx++) {
        etl::to_string(rxBuf.data[idx], message, true);
    }
    LOG_INFO << message.c_str();
}