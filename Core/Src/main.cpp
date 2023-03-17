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
#include "BCHDecoder.hpp"
#include "OQPSKTranscoder.hpp"
#include "LDPCEncoder.hpp"
#include "GMSKTranscoder.hpp"
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

    uint32_t start_time, end_time, total_time = 0;
    input = "0110011100110011001001100001100110100111010110100101011111110001000111010100000110101011111000011011101010000111101100101001100100111110010011000110111101101101001100100110111001010001011111001000010100000111011111001111010101111111100111101010101010010001111100000110100010111010000101011000010010001010010111110100110111111110000011110100011111100100000111010011010010000100110000101101010001111101101010100110011001100110101000000101110010010001011000110110010011000010101101011101011010111110000100110010001111101000011000010101011001001111000101011010010100010101010001100010001110001101000000101100111001111101001000000111001111001001101011110111000010111111101011011110010110010001000001111100101010010110001011110101101100101110011011101111000101110110000000001111000011010100001001010010110111101100101101111100111010111000101000111011111110000001001111011100110010100101110010010100100011000011101110011111001001001001111000111010100110001000110011010010011001010001101000010110001101001001100011010011101101001100111001000110011100000111010001101100100000110011110101001101001011011110010011010010111000000011000101000100001100010001000010011100110001110101001101011000010101000011010110100111101101001010011010110011001000110101011110111100010011110001100101011111001100011100011110011111110000011101111100010000110010000100010000011001100010110110100000001100001001000100011111011111101110111010010010011011100111011001000100111111110100100110111111111111001100100101010001100110111000010001101010101100011111011011101101010001000011011111001111110010101001010000111000010110010011110001101100101100011100100001001111110110101101101000011010011111110011011000111010100000100000111011111011111110110111011011100011001010010100111001111110101100100111000011010100101010001000101001101001100011110010100000101110110011011011010110011101110001100001100001110100011010011001000010100100000110011000100010000100101010111011010001000110100011110000000100100011111000110111101011001111000000110100100101010100010001100111000001101010000110100";

    for (;;) {
//        start_time = xTaskGetTickCount();
//        end_time = xTaskGetTickCount();
        start_time = HAL_GetTick();

        convolutional.encode(input, output);

        end_time = HAL_GetTick();

        total_time = end_time - start_time;

        etl::string<20> str = "Convol: ";
        etl::string<10> breakline = "\r\n";
        uartGatekeeperTask->addToQueue(etl::to_string(total_time, str, true));
        uartGatekeeperTask->addToQueue(breakline);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartBCH(void * pvParameters) {
    uint64_t input[4] = {0b1011101010101010101010101010101010101010101010101010101001010000,
                         0b1110101010101010101010101010101010101010101010101010101001010000,
                         0b1010101010101010101010101010101010101010101010101010101001010000,
                         0b1111101010101010101010101010101010101010101010101010101001010000};
    ;
    BCHDecoder bchDecoder;

    uint32_t start_time, end_time, total_time = 0;
    uint16_t j = 0;
    for(;;) {
        j++;
        etl::optional<uint64_t> output[4];
        start_time = HAL_GetTick();


        for (uint16_t i = 0; i < j; i++) {
            output[i] = bchDecoder.decodeBCH(input[i]);
        }

//        output[0] = bchDecoder.decodeBCH(input[0]);
        end_time = HAL_GetTick();

        total_time = end_time - start_time;
//        if (total_time == 0) continue;
        etl::string<32> str = "BCHDecoder5: ";
        etl::string<20> breakline = "\r\n";
//        if (total_time > 0) {
        uartGatekeeperTask->addToQueue(etl::to_string(total_time, str, true));
        uartGatekeeperTask->addToQueue(etl::to_string(j, breakline, true));
    //    }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartOQPSKRepeated(void * pvParameters) {
    static constexpr uint8_t packetLength = 8;
    static constexpr uint8_t numOfPackets = 5;
    static constexpr uint32_t symbolRate = 12000;
    OQPSKTranscoder<packetLength*numOfPackets> oqpsk(symbolRate);

    double inPhaseSignal[oqpsk.samplesPerSymbol*packetLength*numOfPackets] = {0};
    double quadratureSignal[oqpsk.samplesPerSymbol*packetLength*numOfPackets] = {0};

    uint32_t start_time, end_time, total_time = 0;

    for (;;) {
        //repeated packet
        const etl::bitset<packetLength * numOfPackets> input1 = 0b1010010110100101101001011010010110100101;
        uint32_t start_time, end_time, total_time = 0;

        start_time = HAL_GetTick();
        oqpsk.modulate(input1, inPhaseSignal, quadratureSignal);
        end_time = HAL_GetTick();

        total_time = end_time - start_time;

        etl::string<20> str = "OQPSK Repeated: ";
        etl::string<10> breakline = "\r\n";

        uartGatekeeperTask->addToQueue(etl::to_string(total_time, str, true));
        uartGatekeeperTask->addToQueue(breakline);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartOQPSKRandom(void * pvParameters) {
    static constexpr uint8_t packetLength = 8;
    static constexpr uint8_t numOfPackets = 5;
    static constexpr uint32_t symbolRate = 12000;
    OQPSKTranscoder<packetLength*numOfPackets> oqpsk(symbolRate);

    double inPhaseSignal[oqpsk.samplesPerSymbol*packetLength*numOfPackets] = {0};
    double quadratureSignal[oqpsk.samplesPerSymbol*packetLength*numOfPackets] = {0};

    uint32_t start_time, end_time, total_time = 0;

    for (;;) {
        //random packet
        const etl::bitset<packetLength * numOfPackets> input1 = 0b1010010110100101101001011010010110100101;
        uint32_t start_time, end_time, total_time = 0;

        start_time = HAL_GetTick();
        oqpsk.modulate(input1, inPhaseSignal, quadratureSignal);
        end_time = HAL_GetTick();

        total_time = end_time - start_time;

        etl::string<32> str = "OQPSK Random: ";
        etl::string<10> breakline = "\r\n";

        uartGatekeeperTask->addToQueue(etl::to_string(total_time, str, true));
        uartGatekeeperTask->addToQueue(breakline);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartGMSK(void * pvParameters) {
    const uint16_t PacketLength = 160;
    const uint16_t NumPackets = 1;
    const uint8_t samplesPerSymbol = 10;
    uint32_t sample_frequency = 48000;
    uint32_t symbol_rate = sample_frequency / samplesPerSymbol;

    uint32_t start_time, end_time, total_time = 0;

    GMSKTranscoder<samplesPerSymbol, NumPackets * PacketLength> gmsk_mod(sample_frequency, symbol_rate, false);
    etl::bitset<NumPackets * PacketLength> gmskIn = etl::bitset<NumPackets * PacketLength>();
    etl::bitset<NumPackets * PacketLength> demodSignal = etl::bitset<NumPackets * PacketLength>();
    double gmsk_iout[samplesPerSymbol * PacketLength * NumPackets] = {0};
    double gmsk_qout[samplesPerSymbol * PacketLength * NumPackets] = {0};
    for (;;) {
        for (int i = 0; i < NumPackets; i++) {
            for (int j = 0; j < PacketLength; j++) {
                gmskIn[PacketLength * i + j] = std::rand() % 2;
//                gmskIn[PacketLength * i + j] = 1;
            }
        }
//        etl::string<32> gothere = "Got here!\r\n";
//        uartGatekeeperTask->addToQueue(gothere);
        start_time = HAL_GetTick();
        gmsk_mod.modulate(gmskIn, gmsk_iout, gmsk_qout);
        end_time = HAL_GetTick();
        total_time = end_time - start_time;

        etl::string<32> str1 = "GMSK Modulate1: ";
//        etl::string<32> str2 = "GMSK Demodulate: ";
        etl::string<10> breakline = "\r\n";

        uartGatekeeperTask->addToQueue(etl::to_string(total_time, str1, true));
        uartGatekeeperTask->addToQueue(breakline);

//        start_time = HAL_GetTick();
//        gmsk_mod.demodulate(gmsk_iout, gmsk_qout, samplesPerSymbol * NumPackets * PacketLength, demodSignal);
//        end_time = HAL_GetTick();
//
//        total_time = end_time - start_time;
//        uartGatekeeperTask->addToQueue(etl::to_string(total_time, str2, true));
//        uartGatekeeperTask->addToQueue(breakline);
//        uartGatekeeperTask->addToQueue(breakline);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }


}

void uartLDPC(void * pvParameters) {
    etl::bitset<LDPCEncoder::inputLength> input;
    etl::bitset<LDPCEncoder::outputLength> output;
    LDPCEncoder ldpc;
    uint32_t start_time, end_time, total_time = 0;

    input = ("1111111101111111101111110011111111011111010111111001111100011111111011110110111110101111001011111100111101001111100011110000111111110111011101111011011100110111110101110101011110010111000101111110011101100111101001110010011111000111010001111000011100000111111110110111101110111011001110111101101101011011100110110001101111101011011010111010101100101011110010110100101110001011000010111111001101110011101100110011001111010011010100111001001100010011111000110110001110100011001000111100001101000011100000110000001111111101011111011011110100111101110111010101110110011101000111011110110101101101101011010010110111001101010011011000110100001101111101010111010110110101001101011101010101010101100101010001010111100101011001011010010100100101110001010100010110000101000001011111100101111001101110010011100111011001010110011001100100011001111010010110100110101001001010011100100101001001100010010000100111110001011100011011000100110001110100010101000110010001000100011110000101100001101000010010000111000001010000011000000100000001111111100111111010111110001111101101111001011110100111100001111011101110011011101010111000101110110011100100111010001110000011101111011001110110101101100011011011010110010101101001011000010110111001100110011010100110001001101100011001000110100001100000011011111010011110101011101000111010110110100101101010011010000110101110101001101010101010100010101011001010010010101000101000001010111100100111001010110010001100101101001001010010100100100001001011100010011000101010001000100010110000100100001010000010000000101111110001111100101111000011110011011100010111001001110000011100111011000110110010101100001011001100110001001100100011000000110011110100011101001011010000110100110101000101010010010100000101001110010001100100101001000010010011000100010001001000010000000100111110000111100010111000001110001101100001011000100110000001100011101000011010001010100000101000110010000100100010001000000010001111000001110000101100000011000011010000010100001001000000010000111000000110000010100000001000001100000001000000100000000000000011111111011111111011111100111111110111110101111110011111000111111110111101101111101011110010111111001111010011111000111100001111111101110111011110110111001101111101011101010111100101110001011111100111011001111010011100100111110001110100011110000111000001111111101101111011101110110011101111011011010110111001101100011011111010110110101110101011001010111100101101001011100010110000101111110011011100111011001100110011110100110101001110010011000100111110001101100011101000110010001111000011010000111000001100000011111111010111110110111101001111011101110101011101100111010001110111101101011011011010110100101101110011010100110110001101000011011111010101110101101101010011010111010101010101011001010100010101111001010110010110100101001001011100010101000101100001010000010111111001011110011011100100111001110110010101100110011001000110011110100101101001101010010010100111001001010010011000100100001001111100010111000110110001001100011101000101010001100100010001000111100001011000011010000100100001110000010100000110000001000000011111111001111110101111100011111011011110010111101001111000011110111011100110111010101110001011101100111001001110100011100000111011110110011101101011011000110110110101100101011010010110000101101110011001100110101001100010011011000110010001101000011000000110111110100111101010111010001110101101101001011010100110100001101011101010011010101010101000101010110010100100101010001010000010101111001001110010101100100011001011010010010100101001001000010010111000100110001010100010001000101100001001000010100000100000001011111100011111001011110000111100110111000101110010011100000111001110110001101100101011000010110011001100010011001000110000001100111101000111010010110100001101001101010001010100100101000001010011100100011001001010010000100100110001000100010010000100000001001111100001111000101110000011100011011000010110001001100000011000111010000110100010101000001010001100100001001000100010000000100011110000011100001011000000110000110100000101000010010000000100001110000001100000101000000010000011000000010000001000000000000000");
    for (;;) {
        start_time = HAL_GetTick();
        ldpc.encode(input, output);
        end_time = HAL_GetTick();

        total_time = end_time - start_time;
        etl::string<20> str = "LDPC: ";
        etl::string<10> breakline = "\r\n";

        uartGatekeeperTask->addToQueue(etl::to_string(total_time, str, true));
        uartGatekeeperTask->addToQueue(breakline);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void uartSynchronizer(void * pvParameters) {

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
//    xTaskCreate(uartConvol, "benchmarkConvol", 1000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
//    xTaskCreate(uartGMSK, "benchmarkGMSK", 3000, nullptr, tskIDLE_PRIORITY + 1, nullptr);
    txUHFTask.emplace(48000, 8000, false);
    txUHFTask->createTask();
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
