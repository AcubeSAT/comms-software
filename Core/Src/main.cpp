#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "DummyTask.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "txUHFTask.hpp"
#include "UARTGatekeeperTask.hpp"
extern SPI_HandleTypeDef hspi1;

ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc2;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart3;

template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}


void uartTask1(void * pvParameters) {
    for(;;)
    {
        etl::string<30> str = "[%d]Task A running\r\n";
        uartGatekeeperTask->addToQueue(str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartTask2(void * pvParameters) {
    for(;;)
    {
        etl::string<30> str = "[%d]Task B running\r\n";
        uartGatekeeperTask->addToQueue(str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void blinkyTask1(void * pvParameters){
    for(;;){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(50);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        HAL_Delay(50);
    }
}

void blinkyTask2(void * pvParameters){
    for(;;){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(300);
    }
}

void MCUTemperatureLoggingTask(void * pvParameters){
    const uint16_t* const ADC_TEMP_3V3_30C =  reinterpret_cast<uint16_t*>(0x1FFF7A2C);
    const uint16_t* const ADC_TEMP_3V3_110C =  reinterpret_cast<uint16_t*>(0x1FFF7A2E);
    const float CALIBRATION_REFERENCE_VOLTAGE = 3.3F;

    const float REFERENCE_VOLTAGE = 3.0F; // supplied with Vref+ or VDDA

    // scale constants to current reference voltage
    float adcCalTemp30C = static_cast<float>(*ADC_TEMP_3V3_30C) * (REFERENCE_VOLTAGE/CALIBRATION_REFERENCE_VOLTAGE);
    float adcCalTemp110C = static_cast<float>(*ADC_TEMP_3V3_110C) * (REFERENCE_VOLTAGE/CALIBRATION_REFERENCE_VOLTAGE);
    for(;;){
        uint16_t adcTempValue = HAL_ADC_GetValue(&hadc2);

        float temperature = (static_cast<float>(adcTempValue) - adcCalTemp30C)/(adcCalTemp110C - adcCalTemp30C) * (110.0F - 30.0F) + 30.0F;
        etl::string<50> string = "Temp is ";
        etl::format_spec format;
        etl::string<10> temperatureString;
        etl::to_string(temperature, temperatureString, format, true);
        string.append(temperatureString);
        string.append(" celcius\n\r");
        uartGatekeeperTask->addToQueue(string);
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}

namespace AT86RF215 {
    AT86RF215 transceiver = AT86RF215(&hspi1, AT86RF215Configuration());
}

extern "C" void main_cpp(){
    uartGatekeeperTask.emplace();
//    xTaskCreate(uartTask1, "uartTask 1", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//   xTaskCreate(uartTask2, "uartTask 2", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
    xTaskCreate(MCUTemperatureLoggingTask, "TemperatureLoggingTask", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    txUHFTask.emplace(48000, 4800, false);
//    txUHFTask->createTask();
    uartGatekeeperTask->createTask();
    auto output = String<ECSSMaxMessageSize>("New ");
    LOG_DEBUG<<output.c_str();
    vTaskStartScheduler();


    /**
     * Uncomment below and comment above for Led task visualization (for STM32H743)
     */
//    xTaskCreate(blinkyTask1, "blinkyTask 2", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    xTaskCreate(blinkyTask2, "blinkyTask 2", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);


    for(;;);

    return;
}

/**
 * @brief This function handles EXTI line[15:10] interrupts.
 */
extern "C" void EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);

    AT86RF215::transceiver.handle_irq();
}
