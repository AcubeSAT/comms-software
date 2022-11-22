#include "TransceiverIQPacketCreation.hpp"
#include "GMSKTranscoder.hpp"
#include "bitset"
#include "filter.hpp"
#include "etl/string.h"
#include "etl/format_spec.h"
#include "etl/to_string.h"


extern SPI_HandleTypeDef hspi3;
extern UART_HandleTypeDef huart3;




void TransceiverIQPacketCreation::execute(){
    for(;;) {
        etl::string<Length*SamplesPerSymbol> string = "\r TestTransceiverBytes: ";
        etl::format_spec format;
        etl::to_string(transceiverData[transmitCounter], string, format, true);
        string.append(" \n\r");
        transmitCounter++;
        HAL_SPI_Transmit(&hspi3, (uint8_t *) string.data(), string.size(), 100);
        HAL_UART_Transmit(&huart3,(uint8_t *) string.data(), string.size(), 100);
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
        HAL_Delay(delayTime);
    }
}