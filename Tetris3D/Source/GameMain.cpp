#include "WindowsMinimal.h"
#include "Exception.h"
#include "TetrisApp.h"
//#include "d3dx12.h"
//#include "FRTEngine.h"
#include "resource.h"
#include "Debug/Debug.h"

using frt::Window;
using frt::Exception;
using frt::Mouse;
using frt::Debug;

#include <iostream>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    try
    {
#endif // _DEBUG
        
        App::Launch<TetrisApp>(hInstance, LoadIcon(hInstance, MAKEINTRESOURCE(WIN_ICON)));

#ifdef _DEBUG
    }
    catch (const Exception& e)
    {
        // TODO: log
        Debug::LogError(e.GetType());
        Debug::LogError(e.What());
        Debug::Flush();
        MessageBox(nullptr, e.What(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        Debug::LogError(e.what());
        Debug::Flush();
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        Debug::Flush();
        MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }

#endif // _DEBUG

    App::Close();

    Debug::Flush();
    return 0;
}