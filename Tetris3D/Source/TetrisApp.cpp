#include "TetrisApp.h"
#include "resource.h"
#include <sstream>

#include "Window.h"
#include "Exception.h"

using frt::Window;
using frt::Exception;
using frt::Mouse;

TetrisApp::TetrisApp(HINSTANCE hInstance)
	: App(1280, 720, "Yey",
		LoadIcon(hInstance, MAKEINTRESOURCE(WIN_ICON))
	)
{
}

int TetrisApp::Start()
{
    while (true)
    {
        if (const auto ecode = Window::ProcessMessages())
        {
            return *ecode;
        }

        Update();
    }

    return 0;
}

void TetrisApp::Update()
{
}
