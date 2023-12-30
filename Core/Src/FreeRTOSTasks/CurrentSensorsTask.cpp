#include "CurrentSensorsTask.hpp"
#include "ina3221.hpp"

void CurrentSensorsTask::execute() {
    auto error = INA3221::Error::NO_ERRORS;

    INA3221::INA3221 currentSensor = INA3221::INA3221(hi2c2, INA3221::INA3221Config(), error);

    while (true) {
        Logger::format.precision(Precision);

        for (int i = 1; i <= 3; i++) {
            auto shuntCurrent = currentSensor.getCurrent(i);
            LOG_DEBUG << "Channel shunt current\t" << i << ": " << shuntCurrent.value();
            auto shuntVoltage = currentSensor.getShuntVoltage(i);
            LOG_DEBUG << "Channel shunt Voltage\t" << i << ": " << shuntVoltage.value();
            auto busVoltage = currentSensor.getBusVoltage(i);
            LOG_DEBUG << "Channel bus Voltage\t" << i << ": " << busVoltage.value();
        }

        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }

}
