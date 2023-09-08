#include "TimeKeepingTask.hpp"

#ifdef STM32
void TimeKeepingTask::convertToTM(const RTC_TimeTypeDef &rtcTime, const RTC_DateTypeDef &rtcDate, tm &time) {
    time.tm_sec = static_cast<int>(rtcTime.Seconds);
    time.tm_min = static_cast<int>(rtcTime.Minutes);
    time.tm_hour = static_cast<int>(rtcTime.Hours);
    time.tm_mday = static_cast<int>(rtcDate.Date);
    time.tm_mon = static_cast<int>(rtcDate.Month) - (int) StmMonthPosOffset;
    time.tm_year = static_cast<int>(rtcDate.Year) + (int) StmYearNegOffset;
}

void TimeKeepingTask::convertToRTCTime(const tm &time, RTC_TimeTypeDef &rtcTime) {
    rtcTime.Hours = static_cast<uint8_t>(time.tm_hour);
    rtcTime.Minutes = static_cast<uint8_t>(time.tm_min);
    rtcTime.Seconds = static_cast<uint8_t>(time.tm_sec);
}

void TimeKeepingTask::convertToRTCDate(const tm &time, RTC_DateTypeDef &rtcDate) {
    rtcDate.Date = static_cast<uint8_t>(time.tm_mday);
    rtcDate.Month = static_cast<uint8_t>(time.tm_mon) + StmMonthPosOffset;
    rtcDate.Year = static_cast<uint8_t>(time.tm_year) - StmYearNegOffset;
}
#endif

void TimeKeepingTask::execute() {
    static tm dateTime;
    setEpoch(dateTime);
#ifndef STM32
    RTC_TimeSet(&dateTime);
#else
    static RTC_TimeTypeDef rtcTime;
    static RTC_DateTypeDef rtcDate;
    convertToRTCTime(dateTime, rtcTime);
    convertToRTCDate(dateTime, rtcDate);

    HAL_RTC_SetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
#endif

    while (true) {
#ifndef STM32
        RTC_TimeGet(&dateTime);
#else
        HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);

        convertToTM(rtcTime, rtcDate, dateTime);
#endif
        setTimePlatformParameters(dateTime);
        printOnBoardTime();
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}

void TimeKeepingTask::printOnBoardTime() {
    UTCTimestamp timestamp = CommonParameters::time.getValue().toUTCtimestamp();
    etl::string<29> printTime = "Time:";
    etl::to_string(timestamp.hour, printTime, true);
    printTime += "-";
    etl::to_string(timestamp.minute, printTime, true);
    printTime += "-";
    etl::to_string(timestamp.second, printTime, true);
    printTime += " -- ";
    etl::to_string(timestamp.day, printTime, true);
    printTime += "/";
    etl::to_string(timestamp.month, printTime, true);
    printTime += "/";
    etl::to_string(timestamp.year, printTime, true);
    LOG_DEBUG << printTime.data();
}

void TimeKeepingTask::setTimePlatformParameters(tm &dateTime) {
    UTCTimestamp timeUTC(dateTime.tm_year + YearBase, dateTime.tm_mon + 1, dateTime.tm_mday, dateTime.tm_hour, dateTime.tm_min, dateTime.tm_sec);
    Time::DefaultCUC timeCUC(timeUTC);
    CommonParameters::time.setValue(timeCUC);
}

void TimeKeepingTask::setEpoch(tm &dateTime) {
    dateTime.tm_sec = EpochTime.tm_sec;
    dateTime.tm_min = EpochTime.tm_min;
    dateTime.tm_hour = EpochTime.tm_hour;
    dateTime.tm_mday = EpochTime.tm_mday;
    dateTime.tm_mon = EpochTime.tm_mon;
    dateTime.tm_year = EpochTime.tm_year - YearBase;
}
