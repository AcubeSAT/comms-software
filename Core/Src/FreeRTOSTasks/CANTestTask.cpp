#include "CANTestTask.hpp"
#include "CAN/ApplicationLayer.hpp"
#include "CANGatekeeperTask.hpp"

void CANTestTask::execute() {

    CAN::CANBuffer_t message = {};
    /**
     * Simple 64 byte message sending
     */

    for (uint8_t idx = 0; idx < CAN::Frame::MaxDataLength; idx++) {
        message.push_back(idx);
    }
    String<ECSSMaxMessageSize> testPayload1("SPONGEBOB SQUAREPANTS!");

    String<ECSSMaxMessageSize> testPayload2("GIATI KLANEI TO GATI!");
    while (true) {
        canGatekeeperTask->switchActiveBus(CAN::ActiveBus::Main);
        CAN::Application::createLogMessage(CAN::NodeIDs::OBC, false, testPayload1.data(), false);
        canGatekeeperTask->switchActiveBus(CAN::ActiveBus::Redundant);

        CAN::Application::createLogMessage(CAN::NodeIDs::OBC, false, testPayload2.data(), false);

        xTaskNotify(canGatekeeperTask->taskHandle, 0, eNoAction);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}