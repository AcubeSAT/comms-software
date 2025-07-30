#include <etl/String.hpp>
#include <ECSS_Definitions.hpp>
#include <Logger.hpp>
#include <iomanip>
#include <string>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ServicePool.hpp"
#include "UARTGatekeeperTask.hpp"
#include "COMMS_Definitions.hpp"
#include "CommonParameters.hpp"

void Logger::log(Logger::LogLevel level, etl::istring &message) {
    etl::string<MaxLogNameSize> levelString;
    etl::string<MaxTickCountStringSize> time;

    if (level <= Logger::trace) {
        levelString.append("trace");
    } else if (level <= Logger::debug) {
        levelString.append("debug");
    } else if (level <= Logger::info) {
        levelString.append("info");
    } else if (level <= Logger::notice) {
        levelString.append("notice");
    } else if (level <= Logger::warning) {
        levelString.append("warning");
    } else if (level <= Logger::error) {
        levelString.append("error");
    } else {
        levelString = "emergency";
    }

    while (levelString.available()) {
        levelString.append(" ");
    }

    etl::to_string(xTaskGetTickCount(), time, format.width(MaxTickCountStringSize), 0);

    etl::string<LOGGER_MAX_MESSAGE_SIZE> output;
    output.append(time.c_str());
    output.append(" [");
    output.append(levelString.c_str());
    output.append("] ");

    etl::string<LogSource::MaximumLettersInSubsystemName + 1> subsystemString = LogSource::currentSubsystem;
    if (LogSource::currentSubsystem.empty()) {
        subsystemString = "None ";
    }
    while (subsystemString.available()) {
        subsystemString.append(" ");
    }
    output.append(subsystemString.c_str());

    output.append(message.c_str());
    output.append("\n");

    if (CommonParameters::useUART.getValue()) {
        if (uartGatekeeperTask) {
            uartGatekeeperTask->addToQueue(output);
        }
    }
}


// Reimplementation of the log function for C++ strings
// This is kept in the Platform files, since we don't want to mess with std::strings in the microcontroller
Logger::LogEntry &Logger::LogEntry::operator<<(const std::string &value) {
    message.append(value.c_str());

    return *this;
}

// Logger::LogEntry& Logger::LogEntry::operator<<(char* value) {
//     message.append(value);
//     return *this;
// }
//
// Logger::LogEntry& Logger::LogEntry::operator<<(const char* value) {
//     message.append(value);
//     return *this;
// }
