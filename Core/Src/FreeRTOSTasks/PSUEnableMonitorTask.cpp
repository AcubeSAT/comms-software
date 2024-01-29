#include "FreeRTOSTasks/PSUEnableMonitorTask.hpp"
#include "Task.hpp"
#include "PSU.hpp"

//PSU psu = PSU( GPIOD, GPIO_PIN_9
//        , GPIOC, GPIO_PIN_7
//        , GPIOA, GPIO_PIN_9
//        , GPIOD, GPIO_PIN_8
//        , GPIOE, GPIO_PIN_13);


//FUNCTION CALL FOR comms_eqm_software and cleanroom visits:
PSU psu = PSU( P5V_FPGA_PG_GPIO_Port, P5V_FPGA_PG_Pin
      , P5V_RF_PG_GPIO_Port, P5V_RF_PG_Pin
      , P3V3_RF_PG_GPIO_Port, P3V3_RF_PG_Pin
      , P5V_FPGA_EN_GPIO_Port, P5V_FPGA_EN_Pin
      , P5V_RF_EN_GPIO_Port, P5V_RF_EN_Pin );

void PSUEnableMonitorTask::execute() {

    // 1st enable the different parts of PSU
    psu.enablePartPSU(psu.p5vFPGAenPORT, psu.p5vFPGAenPIN);
    vTaskDelay(pdMS_TO_TICKS(1000)); // settle time
    psu.enablePartPSU(psu.p5vRFenPORT, psu.p5vRFenPIN);
    vTaskDelay(pdMS_TO_TICKS(1000)); // settle time

    if (psu.PGread())
        LOG_INFO << "ALL 3 PG signals are in SET state (P5V_FPGA, P5V_RF, P3V3_RF)";

    // the periodic(3 sec) monitoring of PG signals
    while (true) {
        if (psu.isPinOff(psu.p5vFPGApgPORT, psu.p5vFPGApgPIN)) {
            LOG_INFO << "P5V_FPGA PG signal is NOT in SET state";
            psu.solvePGfault(psu.p5vFPGApgPORT, psu.p5vFPGApgPIN);
        }
        if (psu.isPinOff(psu.p5vRFpgPORT, psu.p5vRFpgPIN)) {
            LOG_INFO << "P5V_RF PG signal is NOT in SET state";
            psu.solvePGfault(psu.p5vRFpgPORT, psu.p5vRFpgPIN);
        }
        if (psu.isPinOff(psu.p3v3RFpgPORT, psu.p3v3RFpgPIN)) {
            LOG_INFO << "P3V3_RF PG signal is NOT in SET state";
            psu.solvePGfault(psu.p3v3RFpgPORT, psu.p3v3RFpgPIN);
        }

        vTaskDelay(pdMS_TO_TICKS(3000));
    }

}