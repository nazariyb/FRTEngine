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

using frt::Window;
using frt::Exception;
using frt::Mouse;
using frt::Event;
using frt::KeyboardEvent;
using frt::MouseEvent;
using frt::Debug;
using frt::Vector2Int;
using frt::Vector3Int;
using frt::Graphics;
using frt::Logger;
using frt::Mesh;

using namespace DirectX;

TetrisApp::TetrisApp()
    : App(1280, 720, "Yey"), object1(nullptr)
{}

int TetrisApp::Start()
{
    object1 = world->SpawnObject<Tetromino>();

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
    };

    window->keyboard.onKeyPressedEvent += [this] (Event* event)
    {
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        window->GetGraphics().OnKeyDown(ev->GetKeyCode());

        if (window->keyboard.IsKeyPressed('T'))
            moveDirections[0] = true;
        if (window->keyboard.IsKeyPressed('F'))
            moveDirections[1] = true;
        if (window->keyboard.IsKeyPressed('G'))
            moveDirections[2] = true;
        if (window->keyboard.IsKeyPressed('H'))
            moveDirections[3] = true;
        if (window->keyboard.IsKeyPressed('R'))
            moveDirections[4] = true;
        if (window->keyboard.IsKeyPressed('Y'))
            moveDirections[5] = true;
    };

    window->keyboard.onKeyReleasedEvent += [this] (Event* event)
    {
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        window->GetGraphics().OnKeyUp(ev->GetKeyCode());

        if (!window->keyboard.IsKeyPressed('T'))
            moveDirections[0] = false;
        if (!window->keyboard.IsKeyPressed('F'))
            moveDirections[1] = false;
        if (!window->keyboard.IsKeyPressed('G'))
            moveDirections[2] = false;
        if (!window->keyboard.IsKeyPressed('H'))
            moveDirections[3] = false;
        if (!window->keyboard.IsKeyPressed('R'))
            moveDirections[4] = false;
        if (!window->keyboard.IsKeyPressed('Y'))
            moveDirections[5] = false;
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

    if (moveDirections[0]) // T
        //Pitch += DirectX::XMConvertToRadians(-2);
        object1->RotatePitch(DirectX::XMConvertToRadians(-2));
    if (moveDirections[1]) // F
        //Roll += DirectX::XMConvertToRadians(2);
        object1->RotateRoll(DirectX::XMConvertToRadians(2));
    if (moveDirections[2]) // G
        //Pitch += DirectX::XMConvertToRadians(2);
        object1->RotatePitch(DirectX::XMConvertToRadians(2));
    if (moveDirections[3]) // H
        //Roll += DirectX::XMConvertToRadians(-2);
        object1->RotateRoll(DirectX::XMConvertToRadians(-2));
    if (moveDirections[4]) // R
        //Yaw += DirectX::XMConvertToRadians(-2);
        object1->RotateYaw(DirectX::XMConvertToRadians(-2));
    if (moveDirections[5]) // Y
        //Yaw += DirectX::XMConvertToRadians(2);
        object1->RotateYaw(DirectX::XMConvertToRadians(2));

    //XMFLOAT4X4 model, view, projection, mvp;
    //FLOAT offsetX = 0.f, offsetZ = 0.f;
    //XMStoreFloat4x4(&model,
    //                XMMatrixMultiply(XMMatrixTranslation(-.5f, 0.f, 0.5f),
    //                                 XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll),
    //                                                  XMMatrixTranslation(offsetX + 0.5f, 0.f, offsetZ - 0.5f))));

    //XMStoreFloat4x4(&view, window->GetGraphics()._camera.GetViewMatrix());
    //XMStoreFloat4x4(&projection, window->GetGraphics()._camera.GetProjectionMatrix(0.8f, window->GetGraphics()._aspectRatio));

    //XMStoreFloat4x4(&mvp, XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection));

    Mesh::SceneObjectConstantBuffer buffer;
    //XMStoreFloat4x4(&buffer.mvp, XMMatrixTranspose(XMLoadFloat4x4(&mvp)));
    //XMStoreFloat4x4(&buffer.modelView, XMMatrixTranspose(XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view)));
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
