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
using frt::MouseEvent;

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

    window.mouse.onButtonPressedEvent += [this](Event* event) {
        std::ostringstream oss;
        oss << (static_cast<MouseEvent*>(event)->IsLeftPressed() ? "Left" : "Right");
        oss << " clicked!";
        window.SetTitle(oss.str());
    };

    window.mouse.onButtonReleasedEvent += [this](Event* event) {
        std::ostringstream oss;
        oss << "Released!";
        window.SetTitle(oss.str());
    };

    window.mouse.onMouseMoveEvent += [this](Event* event) {
        auto mouseEvent = static_cast<MouseEvent*>(event);
        std::ostringstream oss;
        oss << "Move: " << mouseEvent->GetPositionX()
            << " : " << mouseEvent->GetPositionY();
        window.SetTitle(oss.str());
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
    App::Update();
}
