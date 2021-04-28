#pragma once

#include "FRTEngine.h"
#include "WindowsMinimal.h"

#include <profileapi.h>
#include <sysinfoapi.h>
#include <timezoneapi.h>
#include <chrono>


namespace frt
{
class FRTENGINE_API Time
{
public:
    static void Init();
    static void Tick();

    inline static float GetSecondsSinceFirstTick() { return _secondsSinceFirstTick; };

    static float GetCurrentTimeInSeconds();
    static float GetCurrentTimeInMilliSeconds();

    static LPSYSTEMTIME GetCurrentSystemTime();

private:
    static float _secondsSinceFirstTick;
    static std::chrono::steady_clock::time_point _lastTickTime;
};
}
