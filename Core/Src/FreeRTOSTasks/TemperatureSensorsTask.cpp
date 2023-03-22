#include "TemperatureSensorsTask.hpp"
#include "TMP117.hpp"

void TemperatureSensorsTask::execute() {
    auto config = TMP117::Config();
    TMP117::TMP117 tempSensor = TMP117::TMP117(hi2c2, TMP117::I2CAddress::ADDRESS_1, config);
    etl::string<5> temperature;
    etl::format_spec format;
    for(;;){
        std::pair<TMP117::Error, float> temp = tempSensor.getTemperature(true);
        if (temp.first == TMP117::Error::NO_ERRORS){
            etl::to_string(temp.second, temperature, format.precision(2), false);
            LOG_DEBUG << "Temperature is " << temperature.c_str();
        } else {
            LOG_ERROR << "Error getting temperature";
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
