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
    if (HAL_GPIO_ReadPin(P5V_FPGA_EN_GPIO_Port, P5V_FPGA_EN_Pin) == GPIO_PIN_RESET )
        PSU::PSU::enable_FPGA_PSU();
    if (HAL_GPIO_ReadPin(P5V_RF_EN_GPIO_Port, P5V_RF_EN_Pin) == GPIO_PIN_RESET )
        PSU::PSU::enable_RF_PSU();

    // the periodic(5 sec) monitoring of PG signals
    while(1) {
        if(PSU::PSU::PG_read())
            LOG_INFO << "ALL 3 PG signals are in SET state (P5V_FPGA, P5V_RF, P3V3_RF)";
        else {
            LOG_INFO << "1 or more PG signals are NOT in SET state (P5V_FPGA, P5V_RF, P3V3_RF)";
            LOG_INFO << "CALLING FUNCTION TO SOLVE IT";
            PSU::PSU::solve_PG_fault();
        }
        HAL_Delay(5000);
    }

}