#pragma once
#include <filesystem>
#include "Utils/Logger/Logger.h"
#include "Tools\DXHelper.h"
// #include <direct.h>
// #include <stringapiset.h>
// #include <wchar.h>
// #include <string.h>
// #include <winnls.h>
// #include <locale>
// #include <codecvt>
#include "WindowsMinimal.h"
// #include <windows.h>
#include <string>
#include <iostream>

class ShaderReader
{
public:
    static inline HRESULT ReadDataFromFile(LPCWSTR filename, byte** data, UINT* size)
    {
        using namespace Microsoft::WRL;

#if WINVER >= _WIN32_WINNT_WIN8
        CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
        extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
        extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
        extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
        extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
        extendedParams.lpSecurityAttributes = nullptr;
        extendedParams.hTemplateFile = nullptr;

        // TCHAR buffer[MAX_PATH] = { 0 };
        // GetModuleFileName( NULL, buffer, MAX_PATH );
        // const std::string::size_type position = std::string( buff ).find_last_of( "\\/" );
        // const std::string currentDir = std::string( buff ).substr( 0, position);
        // const std::string filepath = currentDir + "\\" + filename;  

        // std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");

        wchar_t buffer[MAX_PATH] = { 0 };
        GetModuleFileNameW( NULL, buffer, MAX_PATH );
        std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
        std::wstring Lfilepath = std::wstring(buffer).substr(0, pos) + L"\\" + filename;
        
        // std::filesystem::current_path().wstring();
        // std::wstring Lfilepath = std::filesystem::current_path().wstring() + L"\\" + filename; 

        frt::Logger::LogInfo("shader path: " + std::string((char*)Lfilepath.c_str()));

        Wrappers::FileHandle file(CreateFile2(Lfilepath.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &extendedParams));
#else
        Wrappers::FileHandle file(CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | SECURITY_SQOS_PRESENT | SECURITY_ANONYMOUS, nullptr));
#endif
        if (file.Get() == INVALID_HANDLE_VALUE)
        {
            frt::Logger::Flush();
            throw std::exception();
        }

        FILE_STANDARD_INFO fileInfo = {};
        if (!GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
        {
            throw std::exception();
        }

        if (fileInfo.EndOfFile.HighPart != 0)
        {
            throw std::exception();
        }

        *data = reinterpret_cast<byte*>(malloc(fileInfo.EndOfFile.LowPart));
        *size = fileInfo.EndOfFile.LowPart;

        if (!ReadFile(file.Get(), *data, fileInfo.EndOfFile.LowPart, nullptr, nullptr))
        {
            throw std::exception();
        }

        return S_OK;
    }

private:
    // static inline std::wstring s2ws(const std::string& s)
    // {
    //     int len;
    //     int slength = (int)s.length() + 1;
    //     len = MultiByteToWideChar(0, 0, s.c_str(), slength, 0, 0); 
    //     wchar_t* buf = new wchar_t[len];
    //     MultiByteToWideChar(0, 0, s.c_str(), slength, buf, len);
    //     std::wstring r(buf);
    //     delete[] buf;
    //     return r;
    // }
};
