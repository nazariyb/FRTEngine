#include "Debug.h"
#include "Time/Time.h"

#include <string>
#include <fstream>


using std::string;
using std::ofstream;
using std::to_string;

namespace frt
{
#if defined(_DEBUG)
const string Debug::_logFileName = "..\\Binaries\\x64\\Debug\\Tetris3D\\log.txt"; // TODO
ofstream Debug::_logBuffer{ _logFileName };
#endif // _DEBUG

void Debug::LogInfo(const string& message)
{
#if defined(_DEBUG)
    Log(message, "Info");
#endif // _DEBUG
}

void Debug::LogWarning(const string& message)
{
#if defined(_DEBUG)
    Log(message, "Warning");
#endif // _DEBUG
}

void Debug::LogError(const string& message)
{
#if defined(_DEBUG)
    Log(message, "Error");
#endif // _DEBUG
}

void Debug::Flush()
{
#if defined(_DEBUG)
    _logBuffer.close();
#endif // _DEBUG
}

void Debug::Log(const string& message, const string& messageType)
{
#if defined(_DEBUG)
    if (!_logBuffer.is_open()) _logBuffer.open(_logFileName);

    SYSTEMTIME currentTime = *frt::Time::GetCurrentSystemTime();
    string time = &currentTime == nullptr ? "[error getting time]" :
        to_string(currentTime.wYear)
        + ":" + FormatTwoDigits(currentTime.wMonth)
        + ":" + FormatTwoDigits(currentTime.wDay)
        + " " + FormatTwoDigits(currentTime.wHour)
        + ":" + FormatTwoDigits(currentTime.wMinute)
        + ":" + FormatTwoDigits(currentTime.wSecond);

    _logBuffer << time + " [" + messageType + "] " + message + "\n";
#endif // _DEBUG
}

string Debug::FormatTwoDigits(WORD number)
{
    return number < 10 ? "0" + to_string(number) : to_string(number);
}

}
