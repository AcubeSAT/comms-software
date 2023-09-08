#pragma once
#include "Task.hpp"

#ifdef STM32
extern RTC_HandleTypeDef hrtc;
#endif

class TimeKeepingTask : public Task {
private:
    const uint16_t DelayMs = 1100;

    const static inline uint16_t TaskStackDepth = 2000;

    StackType_t taskStack[TaskStackDepth];

public:
    void execute();

    /**
     * Sets the year base, used to acquire the current time from a tm structure.
     */
    static constexpr uint16_t YearBase = 1900;

#ifdef STM32
    /**
     * RTC_DateTypeDef year negative offset from YearBase, used in conversions between RTC_DateTypeDef and tm structures. 
     */
    static constexpr uint8_t StmYearNegOffset = 100;

    /**
     * RTC_DateTypeDef month positive offset from tm_mon, used in conversions between RTC_DateTypeDef and tm structures. 
     */
    static constexpr uint8_t StmMonthPosOffset = 1;

    /**
     * Converts stm specific time and date structures to tm structure.
     * @param rtcTime stm specific time RTC_TimeTypeDef structure
     * @param rtcDate stm specific date RTC_DateTypeDef structure
     * @param time tm structure
     */
    static void convertToTM(const RTC_TimeTypeDef &rtcTime, const RTC_DateTypeDef &rtcDate, tm &time);

    /**
     * Converts tm structure to stm specific time structure.
     * @param time tm structure
     * @param rtcTime stm specific time RTC_TimeTypeDef structure
     */
    static void convertToRTCTime(const tm &time, RTC_TimeTypeDef &rtcTime);

    /**
     * Converts tm structure to stm specific date structure
     * @param time tm structure
     * @param rtcDate stm specific date RTC_DateTypeDef structure
     */
    static void convertToRTCDate(const tm &time, RTC_DateTypeDef &rtcDate);
#endif

    /**
     * This function sets the epoch time.
     * @param dateTime is a tm struct witch keeps the time from MCU.
     */
    static void setEpoch(tm &dateTime);

    /**
     * This function sets the AcubeSAT's time parameters using a tm struct.
     * @param dateTime is a tm struct witch keeps the time from MCU.
     */
    void setTimePlatformParameters(tm &dateTime);

    /**
     * This function prints the on-board time.
     */
    void printOnBoardTime();

    TimeKeepingTask() : Task("Timekeeping") {}

    void createTask() {
        xTaskCreateStatic(vClassTask<TimeKeepingTask>, this->TaskName, TimeKeepingTask::TaskStackDepth, this,
                          tskIDLE_PRIORITY + 1, this->taskStack, &(this->taskBuffer));
    }

};

inline std::optional<TimeKeepingTask> timeKeepingTask;
