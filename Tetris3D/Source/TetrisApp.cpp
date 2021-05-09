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

#include "BoardBox.h"
#include "Tetromino.h"
#include "Time/Time.h"
#include "Render/MeshPool.h"
#include "TetrisBoard.h"


using namespace DirectX;
using namespace frt;


TetrisApp::TetrisApp()
    : App(1920, 1080, "Tetris3D")
      , _lastTimeCheck{0.f}
      , tetromino(nullptr)
      , _meshPool(nullptr)
      , _board(nullptr)
{
}

TetrisApp::~TetrisApp()
{
    _board->Clear(_meshPool);
    // TODO delete others
}

int TetrisApp::Run()
{
    Mesh::SceneObjectConstantBuffer& buffer = Tetromino::baseBuffer;
    XMStoreFloat3(&buffer.cameraPosition, XMLoadFloat3(&window->GetGraphics()._camera._position));
    XMStoreFloat3(&buffer.lightPosition,
                  //XMVector3Transform(
                  // XMLoadFloat3(&window->GetGraphics()._camera._position),
                  XMVectorSet(0.f, 30.f, 25.f, 1.f));
    //            window->GetGraphics()._camera.GetViewMatrix()));
    XMStoreFloat4(&buffer.diffuseAlbedo, XMVectorSet(.2f, .6f, .2f, 1.f));
    XMStoreFloat4(&buffer.ambient, XMVectorSet(0.25f, 0.25f, 0.25f, 1.f));
    XMStoreFloat3(&buffer.lightColor, XMVectorSet(1.f, 1.f, 0.9f, 1.0f));
    XMStoreFloat3(&buffer.FresnelR0, XMVectorSet(0.02f, 0.02f, 0.02f, 1.0f));
    buffer.roughness = .7f;
    buffer.falloffStart = 10.f;
    buffer.falloffEnd = 100.f;
    // buffer.diffuseIntensity = 1.0f;
    // buffer.attenuationConst = 1.0f;
    // buffer.attenuationLinear = 0.45;
    // buffer.attenuationQuad = 0.075f;
    // buffer.specularIntensity = 10.f;
    // buffer.specularPower = 20.f;

    world->SpawnObject<BoardBox>();
    _meshPool = world->SpawnObject<MeshPool>(10u * 20u);
    _board = world->SpawnObject<TetrisBoard>(10u, 20u, 2.0f);
    tetromino = _board->SpawnTetromino(world, _meshPool);

    window->keyboard.onKeyPressedEvent += [this](Event* event)
    {
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        window->GetGraphics().OnKeyDown(ev->GetKeyCode());

        switch (ev->GetKeyCode())
        {
        case 'T':
            if (tetromino == nullptr) break;
            _board->DropTetromino(tetromino);
            _runOnNextTick = [this]() { this->OnTetrominoLanded(); };
            break;
        case 'F':
            if (tetromino == nullptr) break;
            _board->MoveTetrominoLeft(tetromino);
            break;
        case 'G':
            if (tetromino == nullptr) break;
            if (_board->MoveTetrominoDown(tetromino) == TetrisBoard::UnableToMove)
                OnTetrominoLanded();
            break;
        case 'H':
            if (tetromino == nullptr) break;
            _board->MoveTetrominoRight(tetromino);
            break;
        case 'R':
            if (tetromino == nullptr) break;
            _board->RotateTetrominoCounterclockwise(tetromino);
            break;
        case 'Y':
            if (tetromino == nullptr) break;
            _board->RotateTetrominoClockwise(tetromino);
            break;
        default: break;
        }
    };

    window->keyboard.onKeyReleasedEvent += [this](Event* event)
    {
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        window->GetGraphics().OnKeyUp(ev->GetKeyCode());
    };

    bool running = true;

    window->keyboard.onKeyReleasedEvent += [this, &running](Event* event)
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

void TetrisApp::Start()
{
}

void TetrisApp::Reset()
{
    tetromino = nullptr;
    _board->Clear(_meshPool);
}

void TetrisApp::OnTetrominoLanded()
{
    Tetromino::baseBuffer.progress += .1f;
    if (_board->HarvestTetromino(world, tetromino, _meshPool) == TetrisBoard::GameOver)
    {
        Logger::LogInfo("Game Over");
        Reset();
        return;
    }
    _board->ClearRowsIfNeeded(_meshPool);
    tetromino = _board->SpawnTetromino(world, _meshPool);
}

void TetrisApp::Update()
{
    App::Update();

    if (tetromino == nullptr)
    {
        tetromino = _board->SpawnTetromino(world, _meshPool);
    }

    if (_runOnNextTick != nullptr)
    {
        _runOnNextTick();
        _runOnNextTick = nullptr;
    }

    const float currentTime = Time::GetSecondsSinceFirstTick();
    if (currentTime - _lastTimeCheck > 1.2f)
    {
        _lastTimeCheck = currentTime;
        if (_board->MoveTetrominoDown(tetromino) == TetrisBoard::UnableToMove)
        {
            OnTetrominoLanded();
        }
    }
}
