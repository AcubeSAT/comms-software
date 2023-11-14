#include "TransceiverTask.hpp"
using namespace AT86RF215;

AT86RF215::At86rf215 TransceiverTask::transceiver = AT86RF215::At86rf215(&hspi1, AT86RF215::AT86RF215Configuration());

uint8_t TransceiverTask::checkTheSPI() {
    uint8_t error_t = 0;
    Error err;
    DevicePartNumber dpn = transceiver.get_part_number(err);
    switch (dpn) {
        case DevicePartNumber::AT86RF215:
            LOG_DEBUG << "Part Number : AT86RF215" ;
            break;
        case DevicePartNumber::AT86RF215IQ:
            LOG_DEBUG << "Part Number : AT86RF215IQ" ;
            break;
        case DevicePartNumber::AT86RF215M:
            LOG_DEBUG << "Part Number : AT86RF215M" ;
            break;
        case DevicePartNumber::AT86RF215_INVALID:
            LOG_DEBUG << "Invalid";
            error_t = 1;
            break;
    }
    if(err != NO_ERRORS){
        error_t = 1;
        switch (err) {
            case FAILED_READING_FROM_REGISTER:
                LOG_DEBUG << "Failed reading from register" ;
                break;
            default:
                LOG_DEBUG << "some other error";
        }
    }
    if(error_t == 1)
        transceiver.chip_reset(err);
    return error_t;
}

TransceiverTask::PacketType TransceiverTask::createRandomPacket(uint16_t length) {
    PacketType packet;
    for (std::size_t i = 0; i < length; i++) {
        packet[i] = i;
    }
    return packet;
}


void TransceiverTask::execute() {

    transceiver.chip_reset(error);
    transceiver.setup(error);

    uint16_t currentPacketLength = 1;
    PacketType packet = createRandomPacket(currentPacketLength);

    while(checkTheSPI() != 0);
    while (true){
        transceiver.transmitBasebandPacketsTx(AT86RF215::RF09, packet.data(), currentPacketLength, error);
        LOG_DEBUG << "signal transmitted";
        vTaskDelay(pdMS_TO_TICKS(4000));

    }
}
