#include "CurrentSensorsTask.hpp"
#include "ina3221.hpp"

void CurrentSensorsTask::execute() {
    auto error = INA3221::Error::NO_ERRORS;

//    HAL_Delay(1000);

    INA3221::INA3221 currentSensor = INA3221::INA3221(hi2c2, INA3221::INA3221Config(), error);
//
    LOG_ERROR << "error status " << (uint8_t) error << "\b\r";
    if (error != INA3221::Error::NO_ERRORS) {
        LOG_ERROR << "Config problem\b\r";
//        vTaskDelay(pdMS_TO_TICKS(100000));
    }

//    HAL_Delay(1000);

//    LOG_DEBUG << currentSensor.getConfigRegister().value() << "\b\r";

//    HAL_Delay(1000);

    while (true) {
        HAL_Delay(100);
        auto currConfig = currentSensor.getConfigRegister();
        if (!currConfig.has_value()) {
            LOG_DEBUG << "PROBLEM with Config\r\b";
            continue;
        }
        LOG_DEBUG << "Config " << currConfig.value() << "\r\b";
        HAL_Delay(100);
        LOG_DEBUG << "ManID "<< currentSensor.getManID().value() << "\r\b";
        for (int i = 1; i <= 3; i++) {
            auto shuntCurrent = currentSensor.getCurrent(i);
            HAL_Delay(1000);
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
//        auto shuntVoltage = currentSensor.getShuntVoltage(2);
//        HAL_Delay(1000);
//        if (!shuntVoltage.has_value()) {
//            LOG_ERROR << "Cannot get voltage\r\b";
//            continue;
//        }
//        Logger::format.precision(Precision);
//        LOG_DEBUG << "Channel shunt Voltage " << shuntVoltage.value() << "\r\b";
//        auto dieID = currentSensor.getDieID();
//        auto manID = currentSensor.getManID();
//        LOG_DEBUG << "DieID " << dieID.value() << " vs " << 0x3220 << "\r\b";
//        LOG_DEBUG << "ManID " << manID.value() << " vs " << 0x5449 << "\r\n";
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }

}
