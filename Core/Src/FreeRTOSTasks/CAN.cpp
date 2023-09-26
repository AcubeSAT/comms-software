#include "CAN.hpp"
#include "CAN/ApplicationLayer.hpp"
void CANTestTask::execute() {

    CAN::CANBuffer_t message = {};

    /**
     * Simple 64 byte message sending
     */

    for (uint8_t idx = 0; idx < CAN::Frame::MaxDataLength; idx++) {
        message.push_back(idx);
    }

    while (true) {
        CAN::Application::sendPingMessage(static_cast<CAN::NodeIDs>(0x11), false);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}