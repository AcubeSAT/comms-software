#include "CAN.hpp"

void CANTestTask::execute() {

    CAN::CANBuffer_t message = {};

    /**
     * Simple 64 byte message sending
     */

    for (uint8_t idx = 0; idx < CAN::Frame::MaxDataLength; idx++) {
        message.push_back(idx);
    }

    while (true) {
        CAN::send(CAN::getFrame(&message));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}