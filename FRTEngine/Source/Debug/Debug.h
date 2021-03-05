#pragma once

#include "FRTEngine.h"
#include "WindowsMinimal.h"

#include <string>
#include <fstream>


namespace frt
{
class FRTENGINE_API Debug
{
public:
    static void LogInfo(const std::string& message);
    static void LogWarning(const std::string& message);
    static void LogError(const std::string& message);

    static void Flush();

private:
    static void Log(const std::string& message, const std::string& messageType);
    static std::string FormatTwoDigits(WORD number);

private:
    static const std::string _logFileName;
    static std::ofstream _logBuffer;
};
}

