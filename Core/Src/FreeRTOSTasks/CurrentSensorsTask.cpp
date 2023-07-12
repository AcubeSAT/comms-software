#include "CurrentSensorsTask.hpp"
#include "ina3221.hpp"

void CurrentSensorsTask::execute() {
    auto error = INA3221::Error::NO_ERRORS;


    INA3221::INA3221 currentSensor = INA3221::INA3221(hi2c2, INA3221::INA3221Config(), error);
    LOG_ERROR << "error status " << (uint8_t) error << "\b\r";
    if (error != INA3221::Error::NO_ERRORS) {
        LOG_ERROR << "Config problem\b\r";
    }

    while (true) {
        auto currConfig = currentSensor.getConfigRegister();
        if (!currConfig.has_value()) {
            LOG_DEBUG << "PROBLEM with Config\r\b";
            continue;
        }
        LOG_DEBUG << "Config " << currConfig.value() << "\r\b";
        for (int i = 1; i <= 3; i++) {
            auto shuntCurrent = currentSensor.getCurrent(i);
            auto shuntVoltage = currentSensor.getShuntVoltage(i);
            auto busVoltage = currentSensor.getBusVoltage(i);
            if (!shuntCurrent.has_value()) {
                LOG_ERROR << "Cannot get voltage\r\b";
                continue;
            }
            Logger::format.precision(Precision);
            LOG_DEBUG << "Channel shunt current\t" << i << ": " << shuntCurrent.value() << "\r\b";
            LOG_DEBUG << "Channel shunt Voltage\t" << i << ": " << shuntVoltage.value() << "\r\b";
            LOG_DEBUG << "Channel bus Voltage\t" << i << ": " << busVoltage.value() << "\r\n";
        }

        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }

}
