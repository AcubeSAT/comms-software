#include "TemperatureSensorsTask.hpp"
#include "TMP117.hpp"

void TemperatureSensorsTask::execute() {

    etl::array<sensor, 3> sensors = {
        sensor {TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address3, config),
                         PlatformParameters::commsPCBTemperatureMCU,
                         "PCB MCU"},
        sensor {TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address1, config),
                         PlatformParameters::commsPCBTemperatureUHF,
                         "PCB UHF PA"},
        sensor {TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address4, config),
                         PlatformParameters::commsPCBTemperatureSBAND,
                         "PCB SBAND PA"},
    };

    // for temporarily holding the temperature and/or error
    etl::pair<TMP117::Error, float> temperature;

    while (true) {
        Logger::format.precision(LoggerPrecision);
        for (auto s : sensors) {
            temperature = s.sensorObject.getTemperature(true);
            if (temperature.first == TMP117::Error::NoErrors) {
                LOG_DEBUG << "Temperature at " << s.sensorName << ": " << temperature.second;
                s.platformParameterReference.setValue(temperature.second);
            } else {
                LOG_ERROR << "Could not get temperature at " << s.sensorName << ". Error: "
                          << errorStrings[temperature.first];
            }
        }
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
