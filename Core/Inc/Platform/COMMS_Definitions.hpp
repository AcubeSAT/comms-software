#ifndef STM32H7A3ZIQSETUP_COMMS_DEFINITIONS_HPP
#define STM32H7A3ZIQSETUP_COMMS_DEFINITIONS_HPP

#include <cstdint>
#include "Logger_Definitions.hpp"

/**
 * The size of the queue used to communicate with the UART Gatekeeper task
 */
inline const uint8_t UARTQueueSize = 5;
/**
 * Maximum string size for a LogLevel is 9 characters long
 */
inline const uint8_t MaxLogNameSize = 9;

/**
 * TickType_t is a uint32_t number. Its string representation is at most 10 digits long.
 */
inline const uint8_t MaxTickCountStringSize = 10;
#endif //STM32H7A3ZIQSETUP_COMMS_DEFINITIONS_HPP
