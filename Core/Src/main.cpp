#include "main.h"
#include "FreeRTOS.h"
#include "list.h"
#include "task.h"
#include "DummyTask.h"
#include "at86rf215.hpp"
#include "at86rf215config.hpp"
#include "txUHFTask.hpp"
#include "UARTGatekeeperTask.hpp"
#include "ConvolutionalEncoder.hpp"
extern SPI_HandleTypeDef hspi1;


template<class T>
static void vClassTask(void *pvParameters) {
    (static_cast<T *>(pvParameters))->execute();
}


void uartTask1(void * pvParameters) {
    for(;;)
    {
        etl::string<30> str = "[%d]La new beginnnig\r\n";
        uartGatekeeperTask->addToQueue(str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartTask2(void * pvParameters) {
    for(;;)
    {
        etl::string<30> str = "[%d]change is good\r\n";
        uartGatekeeperTask->addToQueue(str);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//void uart

void uartConvol(void * pvParameters) {
    ConvolutionalEncoder convolutional;
    etl::bitset<ConvolutionalEncoder::inputLength> input;
    etl::bitset<ConvolutionalEncoder::outputLength> output;

    uint32_t time_begin, time_end, total_time = 0;
    input = "0110011100110011001001100001100110100111010110100101011111110001000111010100000110101011111000011011101010000111101100101001100100111110010011000110111101101101001100100110111001010001011111001000010100000111011111001111010101111111100111101010101010010001111100000110100010111010000101011000010010001010010111110100110111111110000011110100011111100100000111010011010010000100110000101101010001111101101010100110011001100110101000000101110010010001011000110110010011000010101101011101011010111110000100110010001111101000011000010101011001001111000101011010010100010101010001100010001110001101000000101100111001111101001000000111001111001001101011110111000010111111101011011110010110010001000001111100101010010110001011110101101100101110011011101111000101110110000000001111000011010100001001010010110111101100101101111100111010111000101000111011111110000001001111011100110010100101110010010100100011000011101110011111001001001001111000111010100110001000110011010010011001010001101000010110001101001001100011010011101101001100111001000110011100000111010001101100100000110011110101001101001011011110010011010010111000000011000101000100001100010001000010011100110001110101001101011000010101000011010110100111101101001010011010110011001000110101011110111100010011110001100101011111001100011100011110011111110000011101111100010000110010000100010000011001100010110110100000001100001001000100011111011111101110111010010010011011100111011001000100111111110100100110111111111111001100100101010001100110111000010001101010101100011111011011101101010001000011011111001111110010101001010000111000010110010011110001101100101100011100100001001111110110101101101000011010011111110011011000111010100000100000111011111011111110110111011011100011001010010100111001111110101100100111000011010100101010001000101001101001100011110010100000101110110011011011010110011101110001100001100001110100011010011001000010100100000110011000100010000100101010111011010001000110100011110000000100100011111000110111101011001111000000110100100101010100010001100111000001101010000110100";

    for (;;) {
//        time_begin = xTaskGetTickCount();
        time_begin = HAL_GetTick();

        convolutional.encode(input, output);

//        time_end = xTaskGetTickCount();
        time_end = HAL_GetTick();

        total_time = time_end - time_begin;

        etl::string<20> str = "Convol: ";
        etl::string<10> breakline = "\r\n";
        uartGatekeeperTask->addToQueue(etl::to_string(total_time, str, true));
        uartGatekeeperTask->addToQueue(breakline);
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

extern UART_HandleTypeDef huart3;

extern "C" void main_cpp(){
    uartGatekeeperTask.emplace();
//    xTaskCreate(uartTask1, "uartTask 1", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    xTaskCreate(uartTask2, "uartTask 2", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
    xTaskCreate(uartConvol, "benchmark", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
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

    for (;;);


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
