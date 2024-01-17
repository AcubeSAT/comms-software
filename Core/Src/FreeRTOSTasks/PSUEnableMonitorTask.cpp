#include "FreeRTOSTasks/PSUEnableMonitorTask.hpp"
#include "Task.hpp"

#include "stm32h7xx_hal_gpio.h"
#include "PSU.hpp"
//#include "../lib/component-drivers/PSU/src/PSU.cpp"

PSU::PSU psu = PSU::PSU( GPIOD, GPIO_PIN_9
                               , GPIOC, GPIO_PIN_7
                               , GPIOA, GPIO_PIN_9
                               , GPIOD, GPIO_PIN_8
                               , GPIOE, GPIO_PIN_13);

void PSUEnableMonitorTask::execute() {

    // 1st enable the different parts of PSU
    psu.enablePartPSU(psu.p5vFPGAenPORT, psu.p5vFPGAenPIN);
    HAL_Delay(200); // settle time
    psu.enablePartPSU(psu.p5vRFenPORT, psu.p5vRFenPIN);
    HAL_Delay(200); // settle time

    if (psu.PGread())
        LOG_INFO << "ALL 3 PG signals are in SET state (P5V_FPGA, P5V_RF, P3V3_RF)";

    // the periodic(3 sec) monitoring of PG signals
    while (1) {
        if(psu.isPinOff(psu.p5vFPGApgPORT, psu.p5vFPGApgPIN)) {
            LOG_INFO << "P5V_FPGA PG signal is NOT in SET state";
            psu.solvePGfault(psu.p5vFPGApgPORT, psu.p5vFPGApgPIN);
        }
        if(psu.isPinOff(psu.p5vRFpgPORT, psu.p5vRFpgPIN)) {
            LOG_INFO << "P5V_RF PG signal is NOT in SET state";
            psu.solvePGfault(psu.p5vRFpgPORT, psu.p5vRFpgPIN);
        }
        if(psu.isPinOff(psu.p3v3RFenPORT, psu.p3v3RFpgPIN)) {
            LOG_INFO << "P3V3_RF PG signal is NOT in SET state";
            psu.solvePGfault(psu.p3v3RFenPORT, psu.p3v3RFpgPIN);
        }

        HAL_Delay(3000);
    }

}