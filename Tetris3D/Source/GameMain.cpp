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
    App* gameApp = nullptr;
#ifdef _DEBUG
    try
    {
#endif // _DEBUG
        
        gameApp = App::CreateGameApp<TetrisApp>();
        gameApp->Init(hInstance, LoadIcon(hInstance, MAKEINTRESOURCE(WIN_ICON)));
        gameApp->Start();

#ifdef _DEBUG
    }
    catch (const Exception& e)
    {
        // TODO: log
        Debug::Flush();
        MessageBox(nullptr, e.What(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        Debug::Flush();
        MessageBox(nullptr, e.what(), "Standrad Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        Debug::Flush();
        MessageBox(nullptr, "No details available", "Unknown Eception", MB_OK | MB_ICONEXCLAMATION);
    }

#endif // _DEBUG

    if (gameApp != nullptr)
        delete gameApp;

    Debug::Flush();
    return 0;
}