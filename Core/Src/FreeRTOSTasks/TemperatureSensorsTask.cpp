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

    std::string errorStrings[] = {"NoErrors",
                            "Timeout",
                            "InvalidEEPROM",
                            "TemperatureHigh",
                            "TemperatureLow",
                            "NoDataReady",
                            "InvalidCalibrationOffset"};

    TMP117::TMP117 tempSensorMCU = TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address3, config);
    TMP117::TMP117 tempSensorUHF = TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address1, config);
    TMP117::TMP117 tempSensorSBAND = TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address4, config);

    while(true){
        etl::pair<TMP117::Error, float> temperatureMCU = tempSensorMCU.getTemperature(true);
        etl::pair<TMP117::Error, float> temperatureUHF = tempSensorUHF.getTemperature(true);
        etl::pair<TMP117::Error, float> temperatureSBAND = tempSensorSBAND.getTemperature(true);

        // Log MCU Temperature
        if (temperatureMCU.first == TMP117::Error::NoErrors){
            Logger::format.precision(LoggerPrecision);
            LOG_DEBUG << "Temperature at address3:0x94 (MCU) is " << temperatureMCU.second;
            PlatformParameters::commsMCUTemperature.setValue(temperatureMCU.second);
        } else {
            LOG_ERROR << "Could not get temperature from address3:0x94 (MCU). Error: "<< errorStrings[temperatureMCU.first];
        }

        // Log UHF Temperature
        if (temperatureUHF.first == TMP117::Error::NoErrors){
            Logger::format.precision(LoggerPrecision);
            LOG_DEBUG << "Temperature at address1:0x90 (UHF PA) is " << temperatureUHF.second ;
            PlatformParameters::commsPCBTemperatureUHF.setValue(temperatureUHF.second);
        } else {
            LOG_ERROR << "Could not get temperature from address1:0x90 (UHF PA). Error: "<< errorStrings[temperatureUHF.first];
        }

        // Log S-BAND Temperature
        if (temperatureSBAND.first == TMP117::Error::NoErrors){
            Logger::format.precision(LoggerPrecision);
            LOG_DEBUG << "Temperature at address4:0x96 (S-BAND PA)"<< " is " << temperatureSBAND.second;
            PlatformParameters::commsPCBTemperatureSBAND.setValue(temperatureSBAND.second);
        } else {
            LOG_ERROR << "Could not get temperature from address4:0x96 (S-BAND PA). Error: "<< errorStrings[temperatureSBAND.first];
        }
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
