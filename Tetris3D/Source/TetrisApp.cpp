#include "TetrisApp.h"
#include "resource.h"
#include <sstream>
#include <Winuser.h>
#include <iostream>

#include "Window.h"
#include "Exception.h"
#include "Input/KeyboardEvent.h"


using frt::Window;
using frt::Exception;
using frt::Mouse;
using frt::Event;
using frt::KeyboardEvent;

TetrisApp::TetrisApp(HINSTANCE hInstance)
	: App(1280, 720, "Yey",
		LoadIcon(hInstance, MAKEINTRESOURCE(WIN_ICON))
	)
{}

int TetrisApp::Start()
{
	window.keyboard.onKeyReleasedEvent += [this](Event* event) {
		auto keyboardEvent = static_cast<KeyboardEvent*>(event);
		if (keyboardEvent->GetKeyCode() == VK_SPACE)
		{
			window.SetTitle("It's space!");
		}
		else
		{
			window.SetTitle("It's NOT space!");
		}
		std::cout << "CODE is " << keyboardEvent->GetKeyCode() << "\n";
	};

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
