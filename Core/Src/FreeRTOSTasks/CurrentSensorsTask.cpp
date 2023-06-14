#include "CurrentSensorsTask.hpp"
#include "ina3221.hpp"

void CurrentSensorsTask::execute() {
    auto error = INA3221::Error::NO_ERRORS;
    auto config = INA3221::INA3221Config();
    INA3221::INA3221 currentSensor = INA3221::INA3221(hi2c2, config, error);

    if (error != INA3221::Error::NO_ERRORS) {
        LOG_ERROR << "Config problem\r\b";
    }

    while (true) {

    }
}