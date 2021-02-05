#include "Time.h"


namespace frt
{
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
