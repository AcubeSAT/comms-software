#include "FreeRTOSTasks/PSUEnableMonitorTask.hpp"
#include "Task.hpp"

#include "stm32h7xx_hal_gpio.h"
#include "PSU.hpp"
#include "../lib/component-drivers/PSU/src/PSU.cpp"

/*
extern PSU::enable_FPGA_PSU();
extern PSU::enable_RF_PSU();
extern PSU::PG_read();
extern PSU::solve_PG_fault(); */

void PSUEnableMonitorTask::execute() {

    // enable the different parts of PSU IF disabled
    if (PSU::PSU::isOff(P5V_FPGA_EN_GPIO_Port, P5V_FPGA_EN_Pin))
        PSU::PSU::enablePartPSU(P5V_FPGA_EN_GPIO_Port, P5V_FPGA_EN_Pin);
    if (PSU::PSU::isOff(P5V_RF_EN_GPIO_Port, P5V_RF_EN_Pin))
        PSU::PSU::enablePartPSU(P5V_RF_EN_GPIO_Port, P5V_RF_EN_Pin);

    HAL_Delay(200); // Give Some Time for the Signals to settle in I guess
    if (PSU::PSU::PG_read())
        LOG_INFO << "ALL 3 PG signals are in SET state (P5V_FPGA, P5V_RF, P3V3_RF)";

    // the periodic(3 sec) monitoring of PG signals
    while (1) {
        if(PSU::PSU::isOff(P5V_FPGA_PG_GPIO_Port, P5V_FPGA_PG_Pin)) {
            LOG_INFO << "P5V_FPGA PG signal is NOT in SET state";
            PSU::PSU::solvePGfault(P5V_FPGA_PG_GPIO_Port, P5V_FPGA_PG_Pin);
        }
        if(PSU::PSU::isOff(P5V_RF_PG_GPIO_Port, P5V_RF_PG_Pin)) {
            LOG_INFO << "P5V_RF PG signal is NOT in SET state";
            PSU::PSU::solvePGfault(P5V_RF_PG_GPIO_Port, P5V_RF_PG_Pin);
        }
        if(PSU::PSU::isOff(P3V3_RF_PG_GPIO_Port, P3V3_RF_PG_Pin)) {
            LOG_INFO << "P3V3_RF PG signal is NOT in SET state";
            PSU::PSU::solvePGfault(P3V3_RF_PG_GPIO_Port, P3V3_RF_PG_Pin);
        }

        HAL_Delay(3000);
    }

}