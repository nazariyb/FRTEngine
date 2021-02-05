#include "WindowsMinimal.h"
#include "Exception.h"
#include "TetrisApp.h"
//#include "d3dx12.h"
//#include "FRTEngine.h"
#include "resource.h"


using frt::Window;
using frt::Exception;
using frt::Mouse;

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
        MessageBox(nullptr, e.What(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "Standrad Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(nullptr, "No details available", "Unknown Eception", MB_OK | MB_ICONEXCLAMATION);
    }

#endif // _DEBUG

    if (gameApp != nullptr)
        delete gameApp;

    return -1;
}