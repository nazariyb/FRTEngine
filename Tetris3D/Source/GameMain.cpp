#include "WindowsMinimal.h"
#include "Exception.h"
#include "TetrisApp.h"
#include "FRTEngine.h"
#include "resource.h"

using frt::Exception;
using frt::Logger;

void Exit()
{
    App::Shutdown();
    Logger::Flush();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    try
    {
#endif
        
        App::Launch<TetrisApp>(hInstance, LoadIcon(hInstance, MAKEINTRESOURCE(WIN_ICON)));

#ifdef _DEBUG
    }
    catch (const Exception& e)
    {
        Logger::DebugLogError(e.GetType());
        Logger::DebugLogError(e.What());
        Logger::Flush();
        MessageBox(nullptr, e.What(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        Logger::DebugLogError(e.what());
        Logger::Flush();
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        Logger::Flush();
        MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }

#endif

    Exit();
    return 0;
}