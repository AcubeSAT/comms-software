#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "DummyTask.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "MCUTemperatureTask.hpp"
#include "txUHFTask.hpp"
#include "UARTGatekeeperTask.hpp"
#include <etl/string.h>
extern SPI_HandleTypeDef hspi1;

extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc2;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart3;

//ADC_HandleTypeDef hadc1;
//TIM_HandleTypeDef htim2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM3_Init(void);

#define ADC_SAMPLES 100

template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}

//UART_HandleTypeDef huart3;
//SPI_HandleTypeDef hspi1;

void uartTask1(void * pvParameters) {
    char count1 = 0;
    for(;;)
    {
        etl::string<30> str = "[%d]Task A running\r\n";
        uartGatekeeperTask->addToQueue(str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartTask2(void * pvParameters) {
    char count2 = 0;
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



namespace AT86RF215 {
    AT86RF215 transceiver = AT86RF215(&hspi1, AT86RF215Configuration());
}

extern "C" void main_cpp(){
    uartGatekeeperTask.emplace();
//    xTaskCreate(uartTask1, "uartTask 1", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    xTaskCreate(uartTask2, "uartTask 2", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    xTaskCreate(MCUTemperatureLoggingTask, "TemperatureLoggingTask", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    txUHFTask.emplace(48000, 4800, false);
//    txUHFTask->createTask();
    mcuTemperatureTask.emplace();


    uartGatekeeperTask->createTask();
    mcuTemperatureTask->createTask();

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
