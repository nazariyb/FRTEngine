#pragma once

#include "FRTEngine.h"
#include "WindowsMinimal.h"

#include <string>
#include <fstream>

namespace DirectX
{
    class XMFLOAT4;
}

namespace frt
{
class FRTENGINE_API Logger
{
public:
    static void DebugLogInfo(const std::string& message);
    static void DebugLogInfo(const std::string& message, const DirectX::XMFLOAT4& vector);
    static void DebugLogWarning(const std::string& message);
    static void DebugLogError(const std::string& message);

    static void LogInfo(const std::string& message);
    static void LogWarning(const std::string& message);
    static void LogError(const std::string& message);

    static void Flush();

private:
    static void Log(const std::string& message, const std::string& messageType, bool isDebug);
    static std::string FormatTwoDigits(WORD number);

private:
    static const std::string _logFileName;
    static std::ofstream _logBuffer;
};
}

