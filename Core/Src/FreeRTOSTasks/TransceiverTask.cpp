#include "TransceiverTask.hpp"

AT86RF215::AT86RF215 TransceiverTask::transceiver = AT86RF215::AT86RF215(&hspi1, AT86RF215::AT86RF215CustomConfiguration());

uint8_t TransceiverTask::checkTheSPI() {
    uint8_t spi_error = 0;
    AT86RF215::DevicePartNumber dpn = transceiver.get_part_number(error);
    if(dpn == AT86RF215::DevicePartNumber::AT86RF215)
        LOG_DEBUG << "SPI OK" ;
    else{
        spi_error = 1;
        LOG_DEBUG << "SPI ERROR" ;
        transceiver.chip_reset(error);
    }
    return spi_error;
}

TransceiverTask::PacketType TransceiverTask::createRandomPacket(uint16_t length) {
    PacketType packet;
    for (std::size_t i = 0; i < length; i++) {
        packet[i] = i;
    }
    return packet;
}

void TransceiverTask::execute() {
    // Check SPI
    while (checkTheSPI() != 0) {
        vTaskDelay(10);
    };

    transceiver.chip_reset(error);
    transceiver.setup(error);
    LOG_DEBUG << "passed chip_reset and setup";

    // Disable baseband cores if needed (by default, they are both turned on)
    if (!enableBBC0){
        uint8_t reg = transceiver.spi_read_8(AT86RF215::BBC0_PC,error);
        transceiver.spi_write_8(AT86RF215::BBC0_PC,reg | 0x4, error);
    }
    if (!enableBBC1){
        uint8_t reg = transceiver.spi_read_8(AT86RF215::BBC1_PC,error);
        transceiver.spi_write_8(AT86RF215::BBC1_PC,reg | 0x4, error);
    }

    uint16_t currentPacketLength = 100;
    PacketType packet = createRandomPacket(currentPacketLength);

    while (true) {

        /** Energy measurement
        transceiver.clear_channel_assessment(AT86RF215::RF09,error);           // sets the tranceiver to state RF_TXPREP (handle_irq() then sets the state
                                                                                  RF_RX and energy measurements begin)
        vTaskDelay(pdMS_TO_TICKS(3));      //wait for handle_irq() to read the measurement
        if (error!=AT86RF215::NO_ERRORS)
            LOG_DEBUG << "Error: " << static_cast<uint8_t>(error) << "\n";    // look enum at at86rf215.hpp for error values
        else
            LOG_DEBUG << "Energy (EDC register): " << transceiver.energy_measurement << "\n";    // range -127..4 dbm
            LOG_DEBUG << "Energy (RSSI register): " << transceiver.get_rssi(AT86RF215::RF09,error) << "\n";
        **/

        bool packet_correct = true;
        // UHF
        if (enableBBC0) {
            if (RxTxUHF){  // Transmission
                transceiver.basebandPacketsTx(AT86RF215::RF09, packet.data(), currentPacketLength, error);
                if (error != AT86RF215::NO_ERRORS){
                    LOG_DEBUG << "UHF: Could not send packet";
                }
                else{
                    LOG_DEBUG << "UHF: Packet sent";
                }
            }
            else {  // Reception
                transceiver.basebandPacketsRx(AT86RF215::RF09, error); // Sets the tranceiver to state RX
                vTaskDelay(pdMS_TO_TICKS(100));                        // Wait for handle_irq() to detect the interrupts and read the packets
                if (transceiver.BBC0_got_rxfe){
                    for (uint16_t i=0; i<currentPacketLength; i++){
                        if (transceiver.received_packet[i+2] != packet.data()[i]){   // For some reason the received bits start on index 2
                            LOG_DEBUG << "UHF: Incorrect Packet Reception";
                            packet_correct = false;
                            break;
                        }
                    }
                    if (packet_correct){
                        LOG_DEBUG << "UHF: Correct Packet Reception";
                    }

                    packet_correct = true;                                   // Reset flags
                    transceiver.BBC0_got_rxfe = false;
                    for (uint16_t i=0; i<currentPacketLength+2; i++){        // Reset packet buffer
                        transceiver.received_packet[i] = 0;
                    }
                }
            }
        }

        // S-BAND
        if (enableBBC1) {
            if (RxTxSBAND){  // Transmission
                transceiver.basebandPacketsTx(AT86RF215::RF24, packet.data(), currentPacketLength, error);
                if (error != AT86RF215::NO_ERRORS){
                    LOG_DEBUG << "S-BAND: Could not send packet";
                }
                else{
                    LOG_DEBUG << "S-BAND: Packet sent";
                }
            }
            else {  // Reception
                transceiver.basebandPacketsRx(AT86RF215::RF24, error); // Sets the tranceiver to state RX
                vTaskDelay(pdMS_TO_TICKS(100));                        // Wait for handle_irq() to detect the interrupts and read the packets
                if (transceiver.BBC1_got_rxfe){
                    for (uint16_t i=0; i<currentPacketLength; i++){
                        if (transceiver.received_packet[i+2] != packet.data()[i]){   // For some reason the received bits start on index 2
                            LOG_DEBUG << "S-BAND: Incorrect Packet Reception";
                            packet_correct = false;
                            break;
                        }
                    }
                    if (packet_correct){
                        LOG_DEBUG << "s-BAND: Correct Packet Reception";
                    }

                    packet_correct = true;                                   // Reset flags
                    transceiver.BBC1_got_rxfe = false;
                    for (uint16_t i=0; i<currentPacketLength+2; i++){        // Reset packet buffer
                        transceiver.received_packet[i] = 0;
                    }
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}



