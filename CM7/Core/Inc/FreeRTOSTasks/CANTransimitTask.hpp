//
// Created by mylo on 10/22/22.
//

#ifndef STM32H755_CANTRANSIMITTASK_H
#define STM32H755_CANTRANSIMITTASK_H

#include "CANDriver.hpp"
#include "Task.hpp"

class CANTransmitTask : public Task{

};

inline std::optional<CANTransmitTask> canTransmitTask;


#endif //STM32H755_CANTRANSIMITTASK_H
