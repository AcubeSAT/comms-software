#ifndef STM32H7A3ZIQSETUP_COMMS_DEFINITIONS_HPP
#define STM32H7A3ZIQSETUP_COMMS_DEFINITIONS_HPP

#include <cstdint>
#include "Logger_Definitions.hpp"

/**
 * The size of the queue used to communicate with the UART Gatekeeper task
 */
inline const uint8_t UARTQueueSize = 5;

#endif //STM32H7A3ZIQSETUP_COMMS_DEFINITIONS_HPP
