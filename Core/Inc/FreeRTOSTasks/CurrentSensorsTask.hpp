#pragma once

#include <etl/optional.h>
#include "Task.hpp"
#include "ina3221.hpp"

extern I2C_HandleTypeDef hi2c2;

class CurrentSensorsTask : public Task {
public:

    /**
     * Functionality of each channel of INA3221 on the comms board.
     */
    enum class Channel : uint8_t {
        FPGA = 0,
        RF_UHF = 1,
        RF_S = 2
    };

    static inline INA3221::Error error = INA3221::Error::NO_ERRORS;
    static inline INA3221::INA3221 currentSensor = INA3221::INA3221(hi2c2, INA3221::INA3221Config(), error);
    INA3221::ChannelMeasurement channelMeasurement;

    /**
     * Prints current, shunt voltage, bus voltage, power consumption of the input channel
     * @param channel input channel
     * @param current enable/disable current display
     * @param shuntVolt enable/disable shunt voltage display
     * @param busVolt enable/disable bus voltage display
     * @param pow enable/disable power consumption display
     */
    void display(Channel channel,
                        bool current, bool shuntVolt, bool busVolt, bool pow);

    void execute();

    CurrentSensorsTask() : Task("Current Sensors") {}

    void createTask() {
        xTaskCreateStatic(vClassTask < CurrentSensorsTask > , this->TaskName,
                          CurrentSensorsTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }

private:
    static constexpr uint16_t DelayMs = 1000;
    static constexpr uint16_t TaskStackDepth = 2000;
    static constexpr uint8_t Precision = 3;

    StackType_t taskStack[TaskStackDepth];
};


inline etl::optional<CurrentSensorsTask> currentSensorsTask;
