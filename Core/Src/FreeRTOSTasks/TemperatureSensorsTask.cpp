#include "TemperatureSensorsTask.hpp"
#include "TMP117.hpp"

void TemperatureSensorsTask::execute() {
    // Create a struct that holds the config settings, and set them
    // Settings assuming we want to just poll the sensor (we don't care about the alerts):
    TMP117::Config config = {
            TMP117::Continuous,
            static_cast<uint8_t>(7),
            TMP117::Samples8,
            true,
            false,
            true
    };

    // for debugging purposes
    char* errorStrings[] = {"NoErrors",
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
            LOG_DEBUG << "Temperature at address " << TMP117::I2CAddress::Address3 << " (MCU)"<< " is " << temperatureMCU.second;
            PlatformParameters::commsMCUTemperature.setValue(temperatureMCU.second);
        } else {
            LOG_ERROR << "Could not get temperature from address" << TMP117::I2CAddress::Address3 << " (MCU).Error: "<< errorStrings[temperatureMCU.first];
        }

        // Log UHF Temperature
        if (temperatureUHF.first == TMP117::Error::NoErrors){
            Logger::format.precision(LoggerPrecision);
            LOG_DEBUG << "Temperature at address " << TMP117::I2CAddress::Address1 << " (UHF PA)"<< " is " << temperatureUHF.second ;
            PlatformParameters::commsPCBTemperatureUHF.setValue(temperatureUHF.second);
        } else {
            LOG_ERROR << "Error getting temperature from address" << TMP117::I2CAddress::Address1 << " (UHF PA).Error: "<< errorStrings[temperatureUHF.first];
        }

        // Log S-BAND Temperature
        if (temperatureSBAND.first == TMP117::Error::NoErrors){
            Logger::format.precision(LoggerPrecision);
            LOG_DEBUG << "Temperature at address " << TMP117::I2CAddress::Address4 << " (S-BAND PA)"<< " is " << temperatureSBAND.second;
            PlatformParameters::commsPCBTemperatureSBAND.setValue(temperatureSBAND.second);
        } else {
            LOG_ERROR << "Error getting temperature from address" << TMP117::I2CAddress::Address4 << "(S-BAND PA).Error: "<< errorStrings[temperatureSBAND.first];
        }
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
