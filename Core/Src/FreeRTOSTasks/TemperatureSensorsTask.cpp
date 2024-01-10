#include "TemperatureSensorsTask.hpp"
#include "TMP117.hpp"

etl::string<TemperatureSensorsTask::maxErrorStringSize> TemperatureSensorsTask::errorString(TMP117::Error error){
    switch(error) {
        case TMP117::NoErrors:
            return "NoErrors";
        case TMP117::Timeout:
            return "Timeout";
        case TMP117::InvalidEEPROM:
            return "InvalidEEPROM";
        case TMP117::TemperatureHigh:
            return "TemperatureHigh";
        case TMP117::TemperatureLow:
            return "TemperatureLow";
        case TMP117::NoDataReady:
            return "NoDataReady";
        case TMP117::InvalidCalibrationOffset:
            return "InvalidCalibrationOffset";
        default:
            break;
    }

    return "UnknownError";
}

etl::string<TemperatureSensorsTask::maxSensorNameSize> TemperatureSensorsTask::sensorName(TMP117::I2CAddress slaveAddress){
    switch(slaveAddress) {
        case TMP117::I2CAddress::Address3:
            return "PCB MCU";
        case TMP117::I2CAddress::Address1:
            return "PCB UHF PA";
        case TMP117::I2CAddress::Address4:
            return "PCB SBAND PA";
        default:
            break;
    }

    return "UNKNOWN ADDRESS";
}

void TemperatureSensorsTask::execute() {

    etl::array<sensor, 3> sensors = {
        sensor {TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address3, config),
                         PlatformParameters::commsPCBTemperatureMCU,
                         sensorName(TMP117::Address3)},
        sensor {TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address1, config),
                         PlatformParameters::commsPCBTemperatureUHF,
                         sensorName(TMP117::I2CAddress::Address1)},
        sensor {TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address4, config),
                         PlatformParameters::commsPCBTemperatureSBAND,
                         sensorName(TMP117::I2CAddress::Address4)},
    };

    // for temporarily holding of a temperature and error
    etl::pair<TMP117::Error, float> temperature;

    while (true) {
        Logger::format.precision(LoggerPrecision);
        for (auto s : sensors) {
            temperature = s.sensorObject.getTemperature(true);
            if (temperature.first == TMP117::Error::NoErrors) {
                LOG_DEBUG << "Temperature at " << s.sensorName->data() << ": " << temperature.second;
                s.platformParameterReference.setValue(temperature.second);
            } else {
                LOG_ERROR << "Could not get temperature at " << s.sensorName->data() << ". Error: "
                          << errorString(temperature.first).data();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}

