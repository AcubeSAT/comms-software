#include "CurrentSensorsTask.hpp"
#include "ina3221.hpp"
#include <etl/string.h>

void CurrentSensorsTask::display(INA3221::INA3221 &sensor, const Channel ch,
                                 const bool current, const bool shuntVolt, const bool busVolt, const bool pow) {
    etl::string<7> chStr;
    switch (ch) {
        case Channel::FPGA:
            chStr.assign("FPGA");
            break;
        case Channel::RF_UHF:
            chStr.assign("RF_UHF");
            break;
        case Channel::RF_S:
            chStr.assign("RF_S");
            break;
        default:
            chStr.assign("ERROR");
    }

    auto chNum = static_cast<uint8_t>(ch);
    if (current) {
        auto shuntCurrent = sensor.getCurrent(chNum);
        LOG_DEBUG << "Channel shunt current\t" << chStr.data() << ": " << shuntCurrent.value() << " mA ";
    }
    if (shuntVolt) {
        auto shuntVoltage = sensor.getShuntVoltage(chNum);
        LOG_DEBUG << "Channel shunt Voltage\t" << chStr.data() << ": " << shuntVoltage.value() << " mV ";
    }
    if (busVolt) {
        auto busVoltage = sensor.getBusVoltage(chNum);
        LOG_DEBUG << "Channel bus Voltage\t" << chStr.data() << ": " << busVoltage.value() << " mV ";
    }
    if (pow) {
        auto power = sensor.getPower(chNum);
        LOG_DEBUG << "Channel power\t\t" << chStr.data() << ": " << power.value() << " mW\n";
    }
}

void CurrentSensorsTask::execute() {
    auto error = INA3221::Error::NO_ERRORS;

    INA3221::INA3221 currentSensor = INA3221::INA3221(hi2c2, INA3221::INA3221Config(), error);

    while (true) {
        Logger::format.precision(Precision);

        display(currentSensor, Channel::FPGA, true, true, true, true);
        display(currentSensor, Channel::RF_UHF, true, true, true, true);
        display(currentSensor, Channel::RF_S, true, true, true, true);

        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }

}
