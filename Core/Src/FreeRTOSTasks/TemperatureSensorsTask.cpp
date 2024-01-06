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

    TMP117::TMP117 tempSensorPCB_MCU = TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address3, config);
    TMP117::TMP117 tempSensorPCB_UHF = TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address1, config);
    TMP117::TMP117 tempSensorPCB_SBAND = TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address4, config);

    while(true){
        etl::pair<TMP117::Error, float> temperaturePCB_MCU = tempSensorPCB_MCU.getTemperature(true);
        etl::pair<TMP117::Error, float> temperaturePCB_UHF = tempSensorPCB_UHF.getTemperature(true);
        etl::pair<TMP117::Error, float> temperaturePCB_SBAND = tempSensorPCB_SBAND.getTemperature(true);

        // Log MCU Temperature
        if (temperaturePCB_MCU.first == TMP117::Error::NoErrors){
            Logger::format.precision(LoggerPrecision);
            LOG_DEBUG << "Temperature at address3:0x94 (PCB MCU) is " << temperaturePCB_MCU.second;
            PlatformParameters::commsPCBTemperatureMCU.setValue(temperaturePCB_MCU.second);
        } else {
            LOG_ERROR << "Could not get temperature from address3:0x94 (PCB MCU). Error: "<< errorStrings[temperaturePCB_MCU.first];
        }

        // Log UHF Temperature
        if (temperaturePCB_UHF.first == TMP117::Error::NoErrors){
            Logger::format.precision(LoggerPrecision);
            LOG_DEBUG << "Temperature at address1:0x90 (UHF PA) is " << temperaturePCB_UHF.second ;
            PlatformParameters::commsPCBTemperatureUHF.setValue(temperaturePCB_UHF.second);
        } else {
            LOG_ERROR << "Could not get temperature from address1:0x90 (UHF PA). Error: "<< errorStrings[temperaturePCB_UHF.first];
        }

        // Log S-BAND Temperature
        if (temperaturePCB_SBAND.first == TMP117::Error::NoErrors){
            Logger::format.precision(LoggerPrecision);
            LOG_DEBUG << "Temperature at address4:0x96 (S-BAND PA)"<< " is " << temperaturePCB_SBAND.second;
            PlatformParameters::commsPCBTemperatureSBAND.setValue(temperaturePCB_SBAND.second);
        } else {
            LOG_ERROR << "Could not get temperature from address4:0x96 (S-BAND PA). Error: "<< errorStrings[temperaturePCB_SBAND.first];
        }
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
