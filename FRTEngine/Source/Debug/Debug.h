#pragma once

#include "FRTEngine.h"
#include "WindowsMinimal.h"

#include <string>
#include <fstream>


#define string std::string

namespace frt
{
class FRTENGINE_API Debug
{
public:
    static void LogInfo(const string& message);
    static void LogWarning(const string& message);
    static void LogError(const string& message);

    static void Flush();

private:
    static void Log(const string& message, const string& messageType);

private:
    static const string _logFileName;
    static std::ofstream _logBuffer;
};
}

#undef string