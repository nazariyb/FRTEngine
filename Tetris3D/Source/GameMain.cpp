#include "WindowsMinimal.h"
#include "Exception.h"
#include "TetrisApp.h"


using frt::Window;
using frt::Exception;
using frt::Mouse;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    try
    {
#endif // _DEBUG
        
        TetrisApp{ hInstance }.Start();

#ifdef _DEBUG
    }
    catch (const Exception& e)
    {
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

    return -1;
#endif // _DEBUG
}