#ifndef STM32H755_CANTRANSIMITTASK_H
#define STM32H755_CANTRANSIMITTASK_H

#include "CANDriver.hpp"
#include "Task.hpp"

class CANTransmitTask : public Task{
public:
    FDCAN_HandleTypeDef hfdcan1;
    hfdcan1.Instance = FDCAN1;
    hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
    hfdcan1.Init.AutoRetransmission = ENABLE;
    hfdcan1.Init.TransmitPause = DISABLE;
    hfdcan1.Init.ProtocolException = DISABLE;
    hfdcan1.Init.NominalPrescaler = 8;
    hfdcan1.Init.NominalSyncJumpWidth = 4;
    hfdcan1.Init.NominalTimeSeg1 = 16;
    hfdcan1.Init.NominalTimeSeg2 = 16;
    hfdcan1.Init.DataPrescaler = 8;
    hfdcan1.Init.DataSyncJumpWidth = 4;
    hfdcan1.Init.DataTimeSeg1 = 16;
    hfdcan1.Init.DataTimeSeg2 = 16;
    hfdcan1.Init.MessageRAMOffset = 0;
    hfdcan1.Init.StdFiltersNbr = 0;
    hfdcan1.Init.ExtFiltersNbr = 0;
    hfdcan1.Init.RxFifo0ElmtsNbr = 4;
    hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.RxFifo1ElmtsNbr = 4;
    hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.RxBuffersNbr = 0;
    hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
    hfdcan1.Init.TxEventsNbr = 0;
    hfdcan1.Init.TxBuffersNbr = 0;
    hfdcan1.Init.TxFifoQueueElmtsNbr = 2;
    hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
    hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
        HAL_FDCAN_Init(hfdcan);




//        const static inline uint16_t TaskStackDepth = 1500;
//
//        StackType_t taskStack[TaskStackDepth];
//
//        void execute();
//
//        CANTransmitTask() : Task("CAN Transmit") {
//            MCAN1_MessageRAMConfigSet(CANDriver::mcan1MessageRAM);
//
//            MCAN1_TxFifoCallbackRegister(CANDriver::txFifoCallback, CANDriver::MCANTransmit);
//            MCAN1_RxFifoCallbackRegister(MCAN_RX_FIFO_0, CANDriver::rxFifo0Callback, CANDriver::MCANReceive);
        }
};

inline std::optional<CANTransmitTask> canTransmitTask;

#endif //OBC_SOFTWARE_CANTRANSMITTASK_HPP

#endif //STM32H755_CANTRANSIMITTASK_H
