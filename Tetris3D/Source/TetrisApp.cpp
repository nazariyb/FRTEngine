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
#include "Render/Mesh.h"
#include <DirectXMath.h>
#include "Tetrimino.h"
#include "Time/Time.h"


using namespace DirectX;
using namespace frt;


TetrisApp::TetrisApp()
    : App(1280, 720, "Tetris3D")
    , object1(nullptr)
    , _lastTimeCheck{0.f}
{}

int TetrisApp::Start()
{
    object1 = world->SpawnObject<Tetromino>(Tetromino::Type::T, 1.0f);

    window->keyboard.onKeyPressedEvent += [this] (Event* event)
    {
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        window->GetGraphics().OnKeyDown(ev->GetKeyCode());

        if (window->keyboard.IsKeyPressed('T'))
            object1->MoveY(2.f);
        if (window->keyboard.IsKeyPressed('F'))
            object1->RotateRoll(XM_PIDIV2);
        if (window->keyboard.IsKeyPressed('G'))
            object1->MoveY(-2.f);
        if (window->keyboard.IsKeyPressed('H'))
            object1->RotateRoll(-XM_PIDIV2);

    };

    window->keyboard.onKeyReleasedEvent += [this] (Event* event)
    {
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        window->GetGraphics().OnKeyUp(ev->GetKeyCode());
    };

    bool running = true;

    window->keyboard.onKeyReleasedEvent += [this, &running] (Event* event)
    {
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        if (ev->GetKeyCode() == VK_ESCAPE)
        {
            running = false;
        }
    };

    world->InitializeGraphicsResources(&(window->GetGraphics()));
    window->GetGraphics().BeforeFirstTick();
    world->BeforeFirstTick();

    while (true && running)
    {
        if (const auto ecode = Window::ProcessMessages())
        {
            Logger::LogInfo("\nExit from App with: " + std::to_string(*ecode));
            return *ecode;
        }

        Update();
        Render();
    }

    Logger::LogInfo("Exit from App");
    return 0;
}

void TetrisApp::Update()
{
    App::Update();

    float currentTime = Time::GetSecondsSinceFirstTick();
    if ((currentTime - _lastTimeCheck) > .7f)
    {
        object1->MoveY(-2.f);
        _lastTimeCheck = currentTime;
    }

    Mesh::SceneObjectConstantBuffer buffer;
    XMStoreFloat4(&buffer.lightPosition, DirectX::XMVector3Transform(
        DirectX::XMVectorSet(-4.f, 4.f, 0.f, 0.f),
        window->GetGraphics()._camera.GetViewMatrix()));
    XMStoreFloat4(&buffer.diffuseColor, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f));
    XMStoreFloat4(&buffer.ambient, DirectX::XMVectorSet(0.15f, 0.15f, 0.15f, 1.0f));
    buffer.diffuseIntensity = 1.0f;
    buffer.attenuationConst = 1.0f;
    buffer.attenuationLinear = 0.045;
    buffer.attenuationQuad = 0.0075f;
    buffer.specularIntesity = 1.f;
    buffer.specularPower = 30.f;

    object1->UpdateConstantBuffers(&buffer);
}
