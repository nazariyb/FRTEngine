#include "Debug.h"
#include "Time/Time.h"

#include <string>
#include <fstream>

using std::string;
using std::ofstream;
using std::to_string;


namespace frt
{
    const string Debug::_logFileName = "log.txt";
    ofstream Debug::_logBuffer{ _logFileName };

    void Debug::LogInfo(const string& message)
    {
        Log(message, "Info");
    }

    void Debug::LogWarning(const string& message)
    {
        Log(message, "Warning");
    }

    void Debug::LogError(const string& message)
    {
        Log(message, "Error");
    }

    void Debug::Flush()
    {
        _logBuffer.close();
    }

    void Debug::Log(const string& message, const string& messageType)
    {
        LPSYSTEMTIME currentTime = frt::Time::GetCurrentSystemTime();
        string time = to_string(currentTime->wYear)
            + ":" + to_string(currentTime->wMonth)
            + ":" + to_string(currentTime->wDay)
            + " " + to_string(currentTime->wHour)
            + ":" + to_string(currentTime->wMinute)
            + ":" + to_string(currentTime->wSecond);

        _logBuffer << "[" + messageType + "] " + time + " " + message + "\n";
    }

}
