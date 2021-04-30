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
#include "Render/MeshPool.h"
#include "TetrisBoard.h"


using namespace DirectX;
using namespace frt;


TetrisApp::TetrisApp()
    : App(1280, 720, "Tetris3D")
    , object1(nullptr)
    , object2(nullptr)
    , _meshPool(nullptr)
    , _lastTimeCheck{0.f}
{}

int TetrisApp::Start()
{
    Mesh::SceneObjectConstantBuffer& buffer = Tetromino::baseBuffer;
    XMStoreFloat4(&buffer.lightPosition, DirectX::XMVector3Transform(
        DirectX::XMVectorSet(0.f, 15.f, 10.f, 0.f),
        window->GetGraphics()._camera.GetViewMatrix()));
    XMStoreFloat4(&buffer.diffuseColor, DirectX::XMVectorSet(1.f, 1.f, 1.f, 1.f));
    XMStoreFloat4(&buffer.ambient, DirectX::XMVectorSet(0.15f, 0.15f, 0.15f, 1.0f));
    buffer.diffuseIntensity = 1.0f;
    buffer.attenuationConst = 1.0f;
    buffer.attenuationLinear = 0.045;
    buffer.attenuationQuad = 0.0075f;
    buffer.specularIntesity = 1.f;
    buffer.specularPower = 30.f;

    _meshPool = world->SpawnObject<MeshPool>(10u * 20u);
    object1 = world->SpawnObject<Tetromino>(Tetromino::Type::T, 1.f,
                                            TetrisBoard::TopBound,
                                            _meshPool);
    object2 = world->SpawnObject<Tetromino>(Tetromino::Type::I, 1.f,
                                            TetrisBoard::TopBound,
                                            _meshPool);

    window->keyboard.onKeyPressedEvent += [this] (Event* event)
    {
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        window->GetGraphics().OnKeyDown(ev->GetKeyCode());

        if (window->keyboard.IsKeyPressed('T'))
            object1->MoveY(2.f);
        if (window->keyboard.IsKeyPressed('F'))
        {
            if (object1->GetLeftBound() - 2.f >= TetrisBoard::LeftBound.x * 2)
            {
                object1->MoveX(-2.f);
            }
        }
        if (window->keyboard.IsKeyPressed('G'))
            object1->MoveY(-2.f);
        if (window->keyboard.IsKeyPressed('H'))
        {
            if (object1->GetRightBound() + 2.f <= TetrisBoard::RightBound.x * 2)
            {
                object1->MoveX(2.f);
            }
        }
        if (window->keyboard.IsKeyPressed('R'))
            object1->RotateRoll(XM_PIDIV2);
        if (window->keyboard.IsKeyPressed('Y'))
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
        if (object1->GetBottomBound() - 2.f <= TetrisBoard::BottomBound.y * 2)
        {
            object1 = world->SpawnObject<Tetromino>(Tetromino::Type::T, 1.f,
                                                    TetrisBoard::TopBound,
                                                    _meshPool);
        }
    }

}
