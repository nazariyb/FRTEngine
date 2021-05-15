#pragma once

#include "FRTEngine.h"
#include "WindowsMinimal.h"
#include <chrono>


namespace frt
{
class FRTENGINE_API Time
{
public:
    static void Init();
    static void Tick();

    inline static float GetSecondsSinceFirstTick() { return _secondsSinceFirstTick; };

    inline static float GetDeltaSeconds() { return _deltaSeconds; };
    
    static float GetCurrentTimeInSeconds();
    static float GetCurrentTimeInMilliSeconds();

    static LPSYSTEMTIME GetCurrentSystemTime();

private:
    static float _secondsSinceFirstTick;
    static float _deltaSeconds;
    static std::chrono::steady_clock::time_point _lastTickTime;
};
}
