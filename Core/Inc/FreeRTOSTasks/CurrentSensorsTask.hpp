#pragma once

#include <etl/optional.h>
#include "Task.hpp"
#include "ina3221.hpp"

extern I2C_HandleTypeDef hi2c2;

class CurrentSensorsTask : public Task {
public:
    /**
     * Functionality of each channel of INA3221 on the comms board
     */
    enum class Channel : uint8_t {
        FPGA = 0,
        RF_UHF = 1,
        RF_S = 2
    };


    /**
     * Tuple which stores everything the driver returns
     * <0> Array of 3 elements which stores the shunt voltages in uV
     * <1> Array of 3 elements which stores the bus voltages in uV
     * <2> Array of 3 elements which stores the currents in uA
     * <3> Array of 3 elements which stores the consumed powers in mW
     */
    INA3221::ChannelMeasurement channelMeasurement;

    /**
     * Prints current, shunt voltage, bus voltage, power consumption of the input channel
     * @param channel input channel
     * @param displayShuntVoltage enable/disable shunt voltage display
     * @param displayBusVoltage enable/disable bus voltage display
     * @param displayCurrent enable/disable current display
     * @param displayPower enable/disable power consumption display
     */
    void display(Channel channell, bool displayShuntVoltage = true, bool displayBusVoltage = true,
                 bool displayCurrent = true, bool displayPower = true);

    void execute();

    CurrentSensorsTask() : Task("Current Sensors") {}

    void createTask() {
        xTaskCreateStatic(vClassTask < CurrentSensorsTask > , this->TaskName,
                          CurrentSensorsTask::TaskStackDepth, this, tskIDLE_PRIORITY + 1,
                          this->taskStack, &(this->taskBuffer));
    }

private:
    static constexpr uint16_t DelayMs = 2000;
    static constexpr uint16_t TaskStackDepth = 2000;
    static constexpr uint8_t Precision = 3;

    StackType_t taskStack[TaskStackDepth];
};


inline etl::optional<CurrentSensorsTask> currentSensorsTask;
