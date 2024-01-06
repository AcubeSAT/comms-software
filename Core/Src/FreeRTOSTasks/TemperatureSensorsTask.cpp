#include "TemperatureSensorsTask.hpp"
#include "TMP117.hpp"

void TemperatureSensorsTask::execute() {
    // This struct holds settings for the sensors. For polling, continuous mode is used
    // (the sensors periodically produce new values). Alert settings are irrelevant for now,
    // since the alert pin was not used.
    TMP117::Config config = {
            TMP117::Continuous,
            static_cast<uint8_t>(7),
            TMP117::Samples8,
            true,
            false,
            true
    };


    TMP117::TMP117 sensors[] ={TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address3, config),  //PCB MCU
                               TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address1, config),  //PCB UHF PA
                               TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address4, config)}; //PCB SBAND PA

    etl::pair<TMP117::Error, float> temperature;
    while(true){
        Logger::format.precision(LoggerPrecision);
        for (uint8_t i; i<3; i++){
            temperature = sensors[i].getTemperature(true);
            if(temperature.first == TMP117::Error::NoErrors){
                LOG_DEBUG << "Temperature at " << sensorNames[i] << ": " << temperature.second;
                PlatformParameters[i].setValue(temperature.second);
            }
            else
                LOG_ERROR << "Could not get temperature at " << sensorNames[i] << ". Error: " << errorStrings[temperature.first];
        };
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
