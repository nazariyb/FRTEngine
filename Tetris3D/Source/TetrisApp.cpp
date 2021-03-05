#include "TetrisApp.h"
#include <sstream>
#include <Winuser.h>
#include <iostream>
#include <string>
#include <vector>

#include "window.h"
#include "Exception.h"
#include "Input/KeyboardEvent.h"
#include "Debug/Debug.h"
#include "MathLib.h"


using frt::Window;
using frt::Exception;
using frt::Mouse;
using frt::Event;
using frt::KeyboardEvent;
using frt::MouseEvent;
using frt::Debug;
using frt::Matrix;


TetrisApp::TetrisApp()
    : App(1280, 720, "Yey")
{}

int TetrisApp::Start()
{
    window->keyboard.onKeyReleasedEvent += [this] (Event* event) {
        auto keyboardEvent = static_cast<KeyboardEvent*>(event);
        if (keyboardEvent->GetKeyCode() == VK_SPACE)
        {
            window->SetTitle("It's space!");
        }
        else
        {
            window->SetTitle("It's NOT space!");
        }
        // fcking visual studio doesn't support std::format yet!?
        Debug::LogInfo("CODE is " + std::to_string(keyboardEvent->GetKeyCode()));
    };

    window->mouse.onButtonPressedEvent += [this] (Event* event) {
        std::ostringstream oss;
        oss << (static_cast<MouseEvent*>(event)->IsLeftPressed() ? "Left" : "Right");
        oss << " clicked!";
        window->SetTitle(oss.str());
    };

    window->mouse.onButtonReleasedEvent += [this] (Event* event) {
        std::ostringstream oss;
        oss << "Released!";
        window->SetTitle(oss.str());
    };

    window->mouse.onMouseMoveEvent += [this] (Event* event) {
        auto mouseEvent = static_cast<MouseEvent*>(event);
        std::ostringstream oss;
        oss << "Move: " << mouseEvent->GetPositionX()
            << " : " << mouseEvent->GetPositionY();
        window->SetTitle(oss.str());
    };

    Matrix<int> m1 = Matrix<int>(3, 3,
                              {
                                  1, 2, 3,
                                  4, 5, 6,
                                  7, 8, 9
                              });
    Matrix<int> m2 = Matrix<int>(3,
                               {
                                   9, 8, 7,
                                   6, 5, 4,
                                   3, 2, 1
                               });
    Debug::LogInfo("\n" + m1.GetAsString());
    Debug::LogInfo("\n" + operator+(m1, m2).GetAsString());
    Debug::LogInfo("\n" + (m1 + m2).GetAsString());
    Debug::LogInfo("\n" + (m1 - m2).GetAsString());
    Debug::LogInfo("\n" + (m2 - m1).GetAsString());

    while (true)
    {
        if (const auto ecode = Window::ProcessMessages())
        {
            Debug::LogWarning("Exit from App with non-zero");
            return *ecode;
        }

        Update();
    }

    Debug::LogInfo("Exit from App");
    return 0;
}

void TetrisApp::Update()
{
    App::Update();
}
