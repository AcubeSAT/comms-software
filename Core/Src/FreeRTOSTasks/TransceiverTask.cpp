#include "TransceiverTask.hpp"

AT86RF215::AT86RF215 TransceiverTask::transceiver = AT86RF215::AT86RF215(&hspi1, AT86RF215::AT86RF215Configuration());


TransceiverTask::PacketType TransceiverTask::createRandomPacket(uint16_t length) {
    PacketType packet;
    for (std::size_t i = 0; i < length; i++) {
        packet[i] = i;
    }
    return packet;
}

void TransceiverTask::setConfiguration(uint16_t pllFrequency, uint8_t pllChannelNumber) {
    if(rfid==1){
        configFrequency.pllFrequency24 = pllFrequency;
        configFrequency.pllChannelNumber24 = pllChannelNumber;
        configFrequency.pllChannelMode24 = AT86RF215::PLLChannelMode::FineResolution2443;
        transceiver.config = configFrequency;
    }
    else if(rfid==0){
        configFrequency.pllFrequency09 = pllFrequency;
        configFrequency.pllChannelNumber09 = pllChannelNumber;
        configFrequency.pllChannelMode09 = AT86RF215::PLLChannelMode::FineResolution450;
        transceiver.config = configFrequency;
    }
}

/*
* The frequency cannot be lower than 377000 as specified in section 6.3.2. The frequency range related
* to Fine Resolution Channel Scheme CNM.CM=1 is from 389.5MHz to 510MHz
*/
uint16_t TransceiverTask::calculatePllChannelFrequency(uint32_t frequency) {
    uint32_t N=0;
    if(rfid==1){
        N = (frequency - 2366000) * 65536 / 26000;
    }
    else if(rfid==0){
        N = (frequency - 377000) * 65536 / 6500;
    }
    return N >> 8;
}

/*
* The frequency cannot be lower than 377000 as specified in section 6.3.2. The frequency range related
* to Fine Resolution Channel Scheme CNM.CM=1 is from 389.5MHz to 510MHz
*/
uint8_t TransceiverTask::calculatePllChannelNumber(uint32_t frequency) {
    uint32_t N=0;
    if(rfid==1){
        N = (frequency - 2366000) * 65536 / 26000;
    }
    else if(rfid==0){
        N = (frequency - 377000) * 65536 / 6500;
    }
    return N & 0xFF;
}
void TransceiverTask::setF(){
    if(rfid==1){
       FrequencyUHF=2443000;
    }
    else if(rfid==0){
        FrequencyUHF=436500;
    }
}
void TransceiverTask::execute() {
    setF();
    setConfiguration(calculatePllChannelFrequency(FrequencyUHF), calculatePllChannelNumber(FrequencyUHF));

    transceiver.chip_reset(error);
    transceiver.setup(error);

    uint16_t currentPacketLength = 44;
    PacketType packet = createRandomPacket(currentPacketLength);

    while (true) {
        if(rfid==1){
            transceiver.transmitBasebandPacketsTx(AT86RF215::RF24, packet.data(), currentPacketLength, error);
            vTaskDelay(pdMS_TO_TICKS(DelayMs));
        }
        else if(rfid==0){
            transceiver.transmitBasebandPacketsTx(AT86RF215::RF09, packet.data(), currentPacketLength, error);
            vTaskDelay(pdMS_TO_TICKS(DelayMs));
        }
    }
}
