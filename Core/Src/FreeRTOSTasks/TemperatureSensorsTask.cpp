#include "TemperatureSensorsTask.hpp"
#include "TMP117.hpp"

void TemperatureSensorsTask::execute() {
    auto config = TMP117::Config();
    TMP117::TMP117 tempSensor = TMP117::TMP117(hi2c2, TMP117::I2CAddress::Address1, config);

    while(true){
        etl::pair<TMP117::Error, float> temperature = tempSensor.getTemperature(true);
        if (temperature.first == TMP117::Error::NoErrors){
            Logger::format.precision(LoggerPrecision);
            LOG_DEBUG << "Temperature at address " << TMP117::I2CAddress::Address1 << " is " << temperature.second;
            PlatformParameters::commsPCBTemperature1.setValue(temperature.second);
        } else {
            LOG_ERROR << "Error getting temperature";
        }
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
