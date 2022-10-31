#ifndef COMMS_SOFTWARE_CANDRIVER_H
#define COMMS_SOFTWARE_CANDRIVER_H

#include <cstdint>
#include "ECSS_Definitions.hpp"
#include "stm32h7xx_hal_fdcan.h"

class CANDriver {
public:
    static inline FDCAN_MsgRamAddressTypeDef fdCanMessageRAM __attribute__((aligned (32)));

    /**
     * Buffer that stores a message that is ready to be sent
     */
    static inline FDCAN_TxHeaderTypeDef txFifo;

    /**
     * Buffer that stores a received message that is to be processed.
     */
    static inline FDCAN_RxHeaderTypeDef rxFifo0; //probably wrong type


    /**
     * Logs a successful CAN Bus transmission.
     * It is registered as a callback to be automatically called by Microchip's HAL whenever
     * there is a message transmission on TX FIFO.
     *
     * @param context The state of the peripheral when the function is called.
     * The above parameter is a uintptr_t type for compatibility with the HAL, and is casted to APPStates.
     */
    static void HAL_FDCAN_TxFifoCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TxFifoITs);

    /**
     *
     * @param hfdcan hfdcan pointer to an FDCAN_HandleTypeDef structure that contains
  *         the configuration information for the specified FDCAN.
     * @param RxFifo0ITs
     */
    static void HAL_FDCAN_RxFifoCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifoITs, FDCAN_RxHeaderTypeDef rxFifo);

    /**
     * Logs messages that are in the Rx buffer
     * @param rxBuf The RX Buffer that the messages are stored
     */
    static void logMessage(const FDCAN_RxHeaderTypeDef &rxBuf);

    /**
     * Decodes the data length code to get the largest expected size of the message.
     *
     * @param dlc The data length code
     * @return The length of the message in bytes
     */
    static uint8_t convertDlcToLength(uint8_t dlc);

    /**
     * Encodes the length of the message body to the smallest data length code that it fits in.
     *
     * @param length The length of the message in bytes
     * @return The data length code
     */
    static uint8_t convertLengthToDLC(uint8_t length);

//    struct rt_device
//    {
//        struct rt_object          parent;                   /**< inherit from rt_object */
//
//        enum rt_device_class_type type;                     /**< device type */
//        uint16_t               flag;                     /**< device flag */
//        uint16_t               open_flag;                /**< device open flag */
//
//        uint8_t                ref_count;                /**< reference count */
//        uint8_t                device_id;                /**< 0 - 255 */
//
//        /* device call back */
//        rt_err_t (*rx_indicate)(rt_device_t dev, rt_size_t size);
//        rt_err_t (*tx_complete)(rt_device_t dev, void *buffer);
//
//    struct stm32_fdcan
//    {
//        struct rt_device dev;
//        FDCAN_HandleTypeDef fdcan{};
//        FDCAN_FilterTypeDef filter{};
//        FDCAN_TxHeaderTypeDef tx_config{};
//        FDCAN_RxHeaderTypeDef rx_config{};
//        volatile uint8_t fifo0{};
//    };
//
//    static struct stm32_fdcan rt_fdcan = {0};

};

#endif //COMMS_SOFTWARE_CANDRIVER_H
