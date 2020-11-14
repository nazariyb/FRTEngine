#include "WindowsMinimal.h"
#include <sstream>

#include "Window.h"
#include "Exception.h"
#include "resource.h"


using frt::Window;
using frt::Exception;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    try
    {
#endif // _DEBUG
        Window wnd(1280, 720, "Yey",
            LoadIcon(hInstance, MAKEINTRESOURCE(WIN_ICON))
            );

        MSG message;
        BOOL getMessageResult;
        while ((getMessageResult = GetMessage(&message, nullptr, 0, 0)) > 0)
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        if (getMessageResult == -1)
        {
            return getMessageResult;
        }

        return message.wParam;
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