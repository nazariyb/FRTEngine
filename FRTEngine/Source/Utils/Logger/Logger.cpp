#include "Logger.h"
#include "Time/Time.h"


using std::string;
using std::ofstream;
using std::to_string;


#define DEBUG_LOG(message, type) Log(message, type, true);

namespace frt
{
const string Logger::_logFileName = "..\\Binaries\\x64\\Debug\\Tetris3D\\log.txt"; // TODO: path
ofstream Logger::_logBuffer{ _logFileName }; // TODO: don't keep the entire log file in memory through entire runtime

void Logger::DebugLogInfo(const string& message)
{
    DEBUG_LOG(message, "Info")
}

void Logger::DebugLogWarning(const string& message)
{
    DEBUG_LOG(message, "Warning")
}

void Logger::DebugLogError(const string& message)
{
    DEBUG_LOG(message, "Error")
}

void Logger::LogInfo(const string & message)
{
    Log(message, "Info", false);
}

void Logger::LogWarning(const string & message)
{
    Log(message, "Warning", false);
}

void Logger::LogError(const string & message)
{
    Log(message, "Error", false);
}

void Logger::Flush()
{
    _logBuffer.close();
}

void Logger::Log(const string & message, const string & messageType, bool isDebug)
{
    if (!_logBuffer.is_open()) _logBuffer.open(_logFileName);

    SYSTEMTIME currentTime = *frt::Time::GetCurrentSystemTime();
    string time = &currentTime == nullptr ? "[error getting time]" :
        to_string(currentTime.wYear)
        + ":" + FormatTwoDigits(currentTime.wMonth)
        + ":" + FormatTwoDigits(currentTime.wDay)
        + " " + FormatTwoDigits(currentTime.wHour)
        + ":" + FormatTwoDigits(currentTime.wMinute)
        + ":" + FormatTwoDigits(currentTime.wSecond);

    string logType = isDebug ? "DBG " : "DPL ";

    _logBuffer << logType + time + " [" + messageType + "] " + message + "\n";
}

string Logger::FormatTwoDigits(WORD number)
{
    return number < 10 ? "0" + to_string(number) : to_string(number);
}
}