#include "CWBeaconTask.hpp"
#include "etl/string.h"
#include "at86rf215.hpp"

void CWBeaconTask::execute() {
    auto message = etl::string<100>("Will you marry me?");
    AT86RF215::Error err;

    while(true) {
        LOG_DEBUG << "[CWBeacon Task] Starting transmission";
        AT86RF215::transceiverUtils.transmitMorseCode(AT86RF215::RF09, err, 20, message.c_str(), message.size());
        if (err != AT86RF215::Error::NO_ERRORS) {
            LOG_DEBUG << "[CWBeacon Task] Got error during transmission";
        } else {
            LOG_DEBUG << "[CWBeacon Task] Finished transmission";
        }
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}