#include "TransceiverTask.hpp"

AT86RF215::AT86RF215 TransceiverTask::transceiver = AT86RF215::AT86RF215(&hspi1, AT86RF215::AT86RF215Configuration());


TransceiverTask::PacketType TransceiverTask::createRandomPacket(uint16_t length) {
    PacketType packet;
    for (std::size_t i = 0; i < length; i++) {
        packet[i] = i;
    }
    return packet;
}

void TransceiverTask::setConfiguration(uint16_t pllFrequency09, uint8_t pllChannelNumber09) {
    configFrequency.pllFrequency09 = pllFrequency09;
    configFrequency.pllChannelNumber09 = pllChannelNumber09;
    configFrequency.pllChannelMode09 = AT86RF215::PLLChannelMode::FineResolution450;
    transceiver.config = configFrequency;
}

/*
* The frequency cannot be lower than 377000 as specified in section 6.3.2. The frequency range related
* to Fine Resolution Channel Scheme CNM.CM=1 is from 389.5MHz to 510MHz
*/
uint16_t TransceiverTask::calculatePllChannelFrequency09(uint32_t frequency) {
    uint32_t N = (frequency - 377000) * 65536 / 6500;
    return N >> 8;
}

/*
* The frequency cannot be lower than 377000 as specified in section 6.3.2. The frequency range related
* to Fine Resolution Channel Scheme CNM.CM=1 is from 389.5MHz to 510MHz
*/
uint8_t TransceiverTask::calculatePllChannelNumber09(uint32_t frequency) {
    uint32_t N = (frequency - 377000) * 65536 / 6500;
    return N & 0xFF;
}

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

void TransceiverTask::execute() {
    setConfiguration(calculatePllChannelFrequency09(FrequencyUHF), calculatePllChannelNumber09(FrequencyUHF));

    transceiver.chip_reset(error);
    transceiver.setup(error);

    uint16_t currentPacketLength = 44;
    PacketType packet = createRandomPacket(currentPacketLength);

    while (true) {
        int delay = 1000; //in ms
        // Check SPI
        while (checkTheSPI() != 0){
            vTaskDelay(10);
        };

        /** Energy measurement
        transceiver.clear_channel_assessment(AT86RF215::RF09,error); //sets the tranceiver to state RF_TXPREP (and presumably,the energy
                                                                                  //measurement is started by the tranceiver)
        vTaskDelay(pdMS_TO_TICKS(delay));  //wait for handle_irq() to read the measurement
        if (error!=AT86RF215::NO_ERRORS)
            LOG_DEBUG << "Error: " << static_cast<uint8_t>(error) << "\n"; //look enum at at86rf215.hpp for error values
        else
            LOG_DEBUG << "Energy: " << transceiver.energy_measurement << "\n"; //range -127..4 (dBm?)
        **/

        /**RXFS,RSFE interrupts and packet reception**/
        transceiver.transmitBasebandPacketsRx(AT86RF215::RF09,error); // sets the tranceiver to state RX
        vTaskDelay(pdMS_TO_TICKS(delay));  //wait for handle_irq() to detect the interreputs and read the packets
        if (error!=AT86RF215::NO_ERRORS)
            LOG_DEBUG << "Error: " << static_cast<uint8_t>(error) << "\n"; //look enum at at86rf215.hpp for error values
        else // use test variables got t_rxfs,got_rxfe to determine if the interrupts occured
            if (transceiver.got_rxfs)
                LOG_DEBUG << "Got rxfs\n";
            if (transceiver.got_rxfe)
                LOG_DEBUG << "Got rxfe\n";
            for (int i=0; i<2047; i++)
                LOG_DEBUG << transceiver.received_packet[i] << " ";
            LOG_DEBUG << "\n";

        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
