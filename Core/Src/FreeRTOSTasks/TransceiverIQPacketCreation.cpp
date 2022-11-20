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
        etl::string<230> string = "\r KikasTransceiverBytes: ";
        etl::format_spec format;
        for(uint8_t i = 0 ; i < Length * SamplesPerSymbol ; i ++){
            etl::to_string(transceiverData[i], string, format, true);
            string.append(" \n\r");
        }
        HAL_SPI_Transmit(&hspi3, (uint8_t *) string.data(), string.size(), 100);
        HAL_UART_Transmit(&huart3,(uint8_t *) string.data(), string.size(), 100);
    }
}