#include "MCUTemperatureTask.hpp"

void MCUTemperatureTask::execute() {
    while (true) {
        HAL_ADC_Start(&hadc2);
        HAL_ADC_PollForConversion(&hadc2, timeoutPollForConversion);
        uint16_t adcValue = HAL_ADC_GetValue(&hadc2);
        float avgSlope = (tsCal2Temp - tsCal1Temp) / (tsCal2 - tsCal1);
        float temperature = avgSlope * (static_cast<float>(adcValue) - tsCal1) + tsCal1Temp;
        Logger::format.precision(formatPrecision);
        LOG_DEBUG << "MCU Internal Temperature is " << temperature;
        vTaskDelay(pdMS_TO_TICKS(delayMs));
        HAL_ADC_Stop(&hadc2);
    }
}