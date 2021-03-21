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

//using namespace frt;

TetrisApp::TetrisApp()
    : App(1280, 720, "Yey")
{}

int TetrisApp::Start()
{
    window->keyboard.onKeyReleasedEvent += [this] (Event* event)
    {
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

    window->keyboard.onKeyPressedEvent += [this] (Event* event)
    {
        if (window->keyboard.IsKeyPressed('W'))
            window->GetGraphics().moveDirections[0] = true;
        if (window->keyboard.IsKeyPressed('A'))
            window->GetGraphics().moveDirections[1] = true;
        if (window->keyboard.IsKeyPressed('S'))
            window->GetGraphics().moveDirections[2] = true;
        if (window->keyboard.IsKeyPressed('D'))
            window->GetGraphics().moveDirections[3] = true;

    };

    window->keyboard.onKeyReleasedEvent += [this] (Event* event)
    {
        if (!window->keyboard.IsKeyPressed('W'))
            window->GetGraphics().moveDirections[0] = false;
        if (!window->keyboard.IsKeyPressed('A'))
            window->GetGraphics().moveDirections[1] = false;
        if (!window->keyboard.IsKeyPressed('S'))
            window->GetGraphics().moveDirections[2] = false;
        if (!window->keyboard.IsKeyPressed('D'))
            window->GetGraphics().moveDirections[3] = false;

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
    //Debug::LogInfo("\n" + m1.GetAsString());
    //Debug::LogInfo("\n" + operator+(m1, m2).GetAsString());
    //Debug::LogInfo("\n" + (m1 + m2).GetAsString());
    //Debug::LogInfo("\n" + (m1 - m2).GetAsString());
    //Debug::LogInfo("\n" + (m2 - m1).GetAsString());

    Vector2Int v1{ 1, 0 };
    Vector2Int v2{ 1, 2 };
    Vector2Int v3{ -2, 1 };
    Vector3Int v4{ -2, 1, 5 };
    Vector3Int v5{ 0, 7, 2 };

    Debug::LogInfo("v1: " + v1.GetAsString());
    Debug::LogInfo("v2: " + v2.GetAsString());
    Debug::LogInfo("v3: " + v3.GetAsString());
    Debug::LogInfo("v4: " + v4.GetAsString());
    Debug::LogInfo("v5: " + v5.GetAsString());
    
    Debug::LogInfo("");
    Debug::LogInfo("v1 + v2: " + operator+(v1, v2).GetAsString());
    //Debug::LogInfo("v1 + v2: " + (v1 + v2).GetAsString());
    Debug::LogInfo("v1 - v2: " + (v1 - v2).GetAsString());
    Debug::LogInfo("v1 * v2: " + std::to_string(v1 * v2));
    Debug::LogInfo("v1 * v3: " + std::to_string(v1 * v3));
    Debug::LogInfo("v1 * v1: " + std::to_string(v1 * v1));
    Debug::LogInfo("v1 length: " + std::to_string(v1.GetMagnitude()));
    //Debug::LogInfo("v1 normalized: " + v1.GetNormalized().GetAsString());
    //Debug::LogInfo("v2 normalized: " + v2.GetNormalized().GetAsString());
    Debug::LogInfo("v3 * 2: " + (v3 * 2).GetAsString());
    //Vector<int>(10);

    Debug::LogInfo("");
    Debug::LogInfo("v4 + v5: " + (v4 + v5).GetAsString());
    Debug::LogInfo("v4 - v5: " + (v4 - v5).GetAsString());
    Debug::LogInfo("v4 * v5: " + std::to_string(v4 * v5));
    Debug::LogInfo("v4 == v5: " + std::to_string(v4 == v5));
    Debug::LogInfo("v4 != v5: " + std::to_string(v4 != v5));

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

    //Debug::LogInfo("Vector3: " + std::to_string(v3.GetX()));

    //Debug::LogInfo("v3 : " + v3.GetAsString());
    //Debug::LogInfo("v4 : " + v4.GetAsString());
    //Debug::LogInfo("v5 : " + v5.GetAsString());
    //Debug::LogInfo("v6 : " + v6.GetAsString());
    //Debug::LogInfo("v7 : " + v7.GetAsString());
    //Debug::LogInfo("v8 : " + v8.GetAsString());
    //Debug::LogInfo("v9 : " + v9.GetAsString());
    //Debug::LogInfo("v10: " + v10.GetAsString());
    //Debug::LogInfo("v11: " + v11.GetAsString());
    //Debug::LogInfo("v12: " + v12.GetAsString());

    while (true)
    {
        if (const auto ecode = Window::ProcessMessages())
        {
            Debug::LogWarning("\nExit from App with: " + std::to_string(*ecode));
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
