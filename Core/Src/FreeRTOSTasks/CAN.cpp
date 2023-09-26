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
    String<ECSSMaxMessageSize> PEOS("Hello World!");
    while (true) {
        CAN::Application::createLogMessage(CAN::NodeIDs::OBC, false, PEOS.data(), false);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}