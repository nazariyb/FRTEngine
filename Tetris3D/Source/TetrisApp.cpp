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
#include "Render/Graphics.h"

using frt::Window;
using frt::Exception;
using frt::Mouse;
using frt::Event;
using frt::KeyboardEvent;
using frt::MouseEvent;
using frt::Debug;
//using frt::Matrix;
//using frt::Vector;
using frt::Vector2Int;
using frt::Vector3Int;
//using frt::Vector3Float;
using frt::Graphics;
using frt::Logger;

//using namespace frt;

TetrisApp::TetrisApp()
    : App(1280, 720, "Yey")
{}

int TetrisApp::Start()
{
    window->keyboard.onKeyReleasedEvent += [this] (Event* event)
    {
        KeyboardEvent* keyboardEvent = static_cast<KeyboardEvent*>(event);
        if (keyboardEvent->GetKeyCode() == VK_SPACE)
        {
            window->SetTitle("It's space!");
        }
        else
        {
            window->SetTitle("It's NOT space!");
        }
        // fcking visual studio doesn't support std::format yet!?
        //Logger::DebugLogInfo("CODE is " + std::to_string(keyboardEvent->GetKeyCode()));
    };

    window->keyboard.onKeyPressedEvent += [this] (Event* event)
    {
#if NEW_FEATURES
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        window->GetGraphics().OnKeyDown(ev->GetKeyCode()); 
#else
        if (window->keyboard.IsKeyPressed('W'))
            window->GetGraphics().moveDirections[0] = true;
        if (window->keyboard.IsKeyPressed('A'))
            window->GetGraphics().moveDirections[1] = true;
        if (window->keyboard.IsKeyPressed('S'))
            window->GetGraphics().moveDirections[2] = true;
        if (window->keyboard.IsKeyPressed('D'))
            window->GetGraphics().moveDirections[3] = true;

        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        if (ev->GetKeyCode() == VK_CONTROL)
        {
            Logger::DebugLogInfo("Rotation disabled");
            window->GetGraphics().bRotate = false;
        }
        if (ev->GetKeyCode() == VK_MENU)
        {
            Logger::DebugLogInfo("Moving disabled");
            window->GetGraphics().bMove = false;
        }
#endif
    };

    window->keyboard.onKeyReleasedEvent += [this] (Event* event)
    {
#if NEW_FEATURES
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        window->GetGraphics().OnKeyUp(ev->GetKeyCode());
#else
        if (!window->keyboard.IsKeyPressed('W'))
            window->GetGraphics().moveDirections[0] = false;
        if (!window->keyboard.IsKeyPressed('A'))
            window->GetGraphics().moveDirections[1] = false;
        if (!window->keyboard.IsKeyPressed('S'))
            window->GetGraphics().moveDirections[2] = false;
        if (!window->keyboard.IsKeyPressed('D'))
            window->GetGraphics().moveDirections[3] = false;

        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        if (ev->GetKeyCode() == VK_CONTROL)
        {
            Logger::DebugLogInfo("Rotation disabled");
            window->GetGraphics().bRotate = true;
        }
        if (ev->GetKeyCode() == VK_MENU)
        {
            Logger::DebugLogInfo("Moving disabled");
            window->GetGraphics().bMove = true;
        }
#endif
    };

    window->mouse.onButtonPressedEvent += [this] (Event* event)
    {
        std::ostringstream oss;
        oss << (static_cast<MouseEvent*>(event)->IsLeftPressed() ? "Left" : "Right");
        oss << " clicked!";
        window->SetTitle(oss.str());
    };

    window->mouse.onButtonReleasedEvent += [this] (Event* event)
    {
        std::ostringstream oss;
        oss << "Released!";
        window->SetTitle(oss.str());
    };

    window->mouse.onMouseMoveEvent += [this] (Event* event)
    {
        auto mouseEvent = static_cast<MouseEvent*>(event);
        std::ostringstream oss;
        oss << "Move: " << mouseEvent->GetPositionX()
            << " : " << mouseEvent->GetPositionY();
        window->SetTitle(oss.str());
    };

    //Matrix<int> m1 = Matrix<int>(3, 3,
    //                          {
    //                              1, 2, 3,
    //                              4, 5, 6,
    //                              7, 8, 9
    //                          });
    //Matrix<int> m2 = Matrix<int>(3,
    //                           {
    //                               9, 8, 7,
    //                               6, 5, 4,
    //                               3, 2, 1
    //                           });
    //Logger::DebugLogInfo("\n" + m1.GetAsString());
    //Logger::DebugLogInfo("\n" + operator+(m1, m2).GetAsString());
    //Logger::DebugLogInfo("\n" + (m1 + m2).GetAsString());
    //Logger::DebugLogInfo("\n" + (m1 - m2).GetAsString());
    //Logger::DebugLogInfo("\n" + (m2 - m1).GetAsString());

    Vector2Int v1{ 1, 0 };
    Vector2Int v2{ 1, 2 };
    Vector2Int v3{ -2, 1 };
    Vector3Int v4{ -2, 1, 5 };
    Vector3Int v5{ 0, 7, 2 };

    Logger::DebugLogInfo("v1: " + v1.GetAsString());
    Logger::DebugLogInfo("v2: " + v2.GetAsString());
    Logger::DebugLogInfo("v3: " + v3.GetAsString());
    Logger::DebugLogInfo("v4: " + v4.GetAsString());
    Logger::DebugLogInfo("v5: " + v5.GetAsString());
    
    Logger::DebugLogInfo("");
    Logger::DebugLogInfo("v1 + v2: " + operator+(v1, v2).GetAsString());
    //Logger::DebugLogInfo("v1 + v2: " + (v1 + v2).GetAsString());
    Logger::DebugLogInfo("v1 - v2: " + (v1 - v2).GetAsString());
    Logger::DebugLogInfo("v1 * v2: " + std::to_string(v1 * v2));
    Logger::DebugLogInfo("v1 * v3: " + std::to_string(v1 * v3));
    Logger::DebugLogInfo("v1 * v1: " + std::to_string(v1 * v1));
    Logger::DebugLogInfo("v1 length: " + std::to_string(v1.GetMagnitude()));
    //Logger::DebugLogInfo("v1 normalized: " + v1.GetNormalized().GetAsString());
    //Logger::DebugLogInfo("v2 normalized: " + v2.GetNormalized().GetAsString());
    Logger::DebugLogInfo("v3 * 2: " + (v3 * 2).GetAsString());
    //Vector<int>(10);

    Logger::DebugLogInfo("");
    Logger::DebugLogInfo("v4 + v5: " + (v4 + v5).GetAsString());
    Logger::DebugLogInfo("v4 - v5: " + (v4 - v5).GetAsString());
    Logger::DebugLogInfo("v4 * v5: " + std::to_string(v4 * v5));
    Logger::DebugLogInfo("v4 == v5: " + std::to_string(v4 == v5));
    Logger::DebugLogInfo("v4 != v5: " + std::to_string(v4 != v5));

    //Vector2Int v1, v2;
    //v1 + v2;

    //Vector3Float v3{};
    //Vector3Float v4{ 3.f };
    //Vector3Float v5{ 1.f, 2.f, 3.f };
    //float f[] = { 3.f, 3.f, 3.f };
    //Vector3Float v6{ f };
    //Vector3Float v7{ std::make_pair(4.f, 4.f), 5.f };
    //Vector3Float v8{ 5.f, std::make_pair(4.f, 4.f) };
    //Vector3Float v9{ std::make_tuple(4.f, 4.f), 5.f };
    //Vector3Float v10{ 5.f, std::make_tuple(4.f, 4.f) };
    //Vector3Float v11{ std::make_tuple(4.f, 4.f, 4.f) };
    //Vector3Float v12{ std::vector{ 6.f, 6.f, 6.f } };

    //frt::Vector3Int::GetZeroVector();

    //Logger::DebugLogInfo("Vector3: " + std::to_string(v3.GetX()));

    //Logger::DebugLogInfo("v3 : " + v3.GetAsString());
    //Logger::DebugLogInfo("v4 : " + v4.GetAsString());
    //Logger::DebugLogInfo("v5 : " + v5.GetAsString());
    //Logger::DebugLogInfo("v6 : " + v6.GetAsString());
    //Logger::DebugLogInfo("v7 : " + v7.GetAsString());
    //Logger::DebugLogInfo("v8 : " + v8.GetAsString());
    //Logger::DebugLogInfo("v9 : " + v9.GetAsString());
    //Logger::DebugLogInfo("v10: " + v10.GetAsString());
    //Logger::DebugLogInfo("v11: " + v11.GetAsString());
    //Logger::DebugLogInfo("v12: " + v12.GetAsString());

    bool running = true;

    window->mouse.onButtonPressedEvent += [this, &running] (Event* event)
    {
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        if (ev->GetKeyCode() == VK_ESCAPE)
        {
            running = false;
        }
    };

    while (true && running)
    {
        if (const auto ecode = Window::ProcessMessages())
        {
            Logger::LogInfo("\nExit from App with: " + std::to_string(*ecode));
            return *ecode;
        }

        Update();
    }

    Logger::LogInfo("Exit from App");
    return 0;
}

void TetrisApp::Update()
{
    App::Update();
}
