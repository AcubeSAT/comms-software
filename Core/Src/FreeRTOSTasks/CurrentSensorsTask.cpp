#include "CurrentSensorsTask.hpp"
#include "etl/string.h"

void CurrentSensorsTask::display(const Channel channel, const bool displayShuntVoltage, const bool displayBusVoltage,
                                 const bool displayCurrent, const bool displayPower) {
    etl::string<7> channelString;
    switch (channel) {
        case Channel::FPGA:
            channelString.assign("FPGA");
            break;
        case Channel::RF_UHF:
            channelString.assign("RF_UHF");
            break;
        case Channel::RF_S:
            channelString.assign("RF_S");
            break;
        default:
            channelString.assign("ERROR");
    }

    auto channelIndex = static_cast<std::underlying_type_t<Channel>>(channel);
    if (displayShuntVoltage) {
        auto shuntVoltage = std::get<0>(channelMeasurement.value())[channelIndex];
        LOG_DEBUG << "Channel shunt Voltage\t" << channelString.data() << ": " << shuntVoltage.value() << " uV ";
    }
    if (displayBusVoltage) {
        auto busVoltage = std::get<1>(channelMeasurement.value())[channelIndex];
        LOG_DEBUG << "Channel bus Voltage\t" << channelString.data() << ": " << busVoltage.value() << " uV ";
    }
    if (displayCurrent) {
        auto current = std::get<2>(channelMeasurement.value())[channelIndex];
        LOG_DEBUG << "Channel shunt current\t" << channelString.data() << ": " << current.value() << " uA ";
    }
    if (displayPower) {
        auto power = std::get<3>(channelMeasurement.value())[channelIndex];
        LOG_DEBUG << "Channel power\t\t" << channelString.data() << ": " << power.value() << " mW";
    }

}

void CurrentSensorsTask::execute() {
    INA3221::Error error = INA3221::Error::NO_ERRORS;
    INA3221::INA3221 currentSensor = INA3221::INA3221(hi2c2, INA3221::INA3221Config(), error);
    Logger::format.precision(Precision);

    while (true) {
        channelMeasurement = currentSensor.getMeasurement();
        if (not channelMeasurement.has_value()) {
            LOG_ERROR << "Could not get current measurements!";
        } else {
            display(Channel::FPGA, true, true, true, true);
            display(Channel::RF_UHF, true, true, true, true);
            display(Channel::RF_S, true, true, true, true);
        }

        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }

}
