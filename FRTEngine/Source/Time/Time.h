#pragma once

#include "FRTEngine.h"
#include "WindowsMinimal.h"

#include <profileapi.h>
#include <sysinfoapi.h>
#include <timezoneapi.h>


namespace frt
{
class FRTENGINE_API Time
{
public:
    static float GetCurrentTimeInSeconds();
    static float GetCurrentTimeInMilliSeconds();

    static LPSYSTEMTIME GetCurrentSystemTime();
};
}
