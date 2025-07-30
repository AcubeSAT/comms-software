#include "ParameterStatisticsService.hpp"

#ifdef SERVICE_PARAMETERSTATISTICS
void ParameterStatisticsService::initializeStatisticsMap() {
    uint8_t idIndex = 0;
    for (auto &statistic: ParameterStatistics::statistics) {
        statisticsMap.insert({ParameterStatistics::statisticParameterIDs[idIndex++], statistic});
    }
}
#endif
