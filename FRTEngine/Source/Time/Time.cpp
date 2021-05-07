#include "Time.h"

namespace frt
{

float Time::_secondsSinceFirstTick = 0.f;
float Time::_deltaSeconds = 0.f;
std::chrono::steady_clock::time_point Time::_lastTickTime{};

void Time::Init()
{
    _lastTickTime = std::chrono::steady_clock::now();
}

void Time::Tick()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    _deltaSeconds = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastTickTime).count() / 1'000'000.f;;
    _secondsSinceFirstTick += _deltaSeconds;
    _lastTickTime = now;
}

float Time::GetCurrentTimeInSeconds()
{
    //LARGE_INTEGER currentTime;
    //QueryPerformanceCounter(&currentTime); // if returns zero - error occured

    return 0.;

}

float Time::GetCurrentTimeInMilliSeconds()
{
    return 0.;
}

SYSTEMTIME* Time::GetCurrentSystemTime()
{
    FILETIME fileTime;
    GetSystemTimeAsFileTime(&fileTime);
    //if (fileTime == nullptr) return nullptr;

    SYSTEMTIME systemTime;
    if (FileTimeToSystemTime(&fileTime, &systemTime) == 0)
        return nullptr;

    return &systemTime;
}
}
