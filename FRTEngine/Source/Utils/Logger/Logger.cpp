#include "Logger.h"
#include "Time/Time.h"
#include <DirectXMath.h>

using std::string;
using std::ofstream;
using std::to_string;
using DirectX::XMFLOAT4;

#if defined(_DEBUG)
#define DEBUG_LOG(message, type) Log((message), (type), true);
#else
#define DEBUG_LOG(message, type)
#endif

namespace frt
{
const string Logger::_logFileName = "..\\Binaries\\x64\\Debug\\Tetris3D\\log.txt"; // TODO: path
ofstream Logger::_logBuffer{ _logFileName }; // TODO: don't keep the entire log file in memory through entire runtime

void Logger::DebugLogInfo(const string& message)
{
    DEBUG_LOG(message, "Info")
}

void Logger::DebugLogInfo(const std::string& message, const XMFLOAT4& vector)
{
    DEBUG_LOG(message + "<"
        + std::to_string(vector.x) + ", "
        + std::to_string(vector.y) + ", "
        + std::to_string(vector.z) + ", "
        + std::to_string(vector.w) + ">", "Info")
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

#if defined(_DEBUG)
    string logType = isDebug ? "DBG " : "RLS ";
#else
    string logType = "";
#endif

    _logBuffer << logType + time + " [" + messageType + "] " + message + "\n";
}

string Logger::FormatTwoDigits(WORD number)
{
    return number < 10 ? "0" + to_string(number) : to_string(number);
}
}