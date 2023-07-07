#include "TransceiverTask.h"


void TransceiverTask::execute() {


    transceiver.chip_reset(error);
    transceiver.setup(error);
    volatile uint16_t  b = transceiver.get_version_number(error);
    uint8_t packet[] = {0, 1, 2, 3, 43, 56, 2, 78, 12, 34, 82, 39};
    while (true) {

        volatile AT86RF215::State state = transceiver.get_state(AT86RF215::RF09, error);
        state;
        transceiver.transmitBasebandPacketsTx(AT86RF215::RF09, packet, 12, error);
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }

}

