#include "TetrisApp.h"
#include <sstream>
#include <Winuser.h>
#include <string>
#include <vector>
#include "window.h"
#include "Input/KeyboardEvent.h"
#include "Debug/Debug.h"
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

const TetrisApp::LevelInfo TetrisApp::Levels[] = {
    {{0.97f, 0.7f, 0.27f, 1.0f}, 1.f, 3u},  // maximum yellow red
    {{0.47f, 0.7f, 0.68f, 1.f}, 3.f, 3u},   // green sheen
    {{0.93f, 0.6f, 0.64f, 1.f}, 5.f, 3u},   // mauvelous
    {{0.95f, 0.51f, 0.19f, 1.f}, 9.f, 3u},  // princeton orange 
    {{0.53f, 0.25f, 0.31f, 1.f}, 2.f, 3u},  // solid pink
    {{0.74f, 0.57f, 0.6f, 1.f}, 10.f, 3u},  // rosy brown
    {{0.95f, 0.89f, 0.89f, 1.f}, 4.f, 3u},  // lavender blush
    {{0.89f, 0.44f, 0.57f, 1.f}, 8.f, 3u},  // cyclamen
    {{0.67f, 0.78f, 0.69f, 1.f}, 6.f, 3u},  // cambridge blue
    {{0.8f, 0.55f, 0.53f, 1.f}, 7.f, 3u},   // old rose
};


TetrisApp::TetrisApp()
    : App(1920, 1080, "Tetris3D")
      , _currentLevel(0)
      , _progress(0)
      , _lastTimeCheck{0.f}
      , _isInputEnabled(true)
      , toDrop(false)
      , toMoveDown(false)
      , toMoveLeft(false)
      , toMoveRight(false)
      , toRotateClockwise(false)
      , toRotateCounterClockwise(false)
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
    XMStoreFloat3(&buffer.lightPosition1, XMVectorSet(-25.f, 30.f, 33.f, 1.f));
    XMStoreFloat3(&buffer.lightPosition2, XMVectorSet(25.f, 30.f, 33.f, 1.f));
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
    tetromino = _board->SpawnTetromino(world, _meshPool, Levels[_currentLevel].color);

    window->keyboard.onKeyPressedEvent += [this](Event* event)
    {
        KeyboardEvent* ev = static_cast<KeyboardEvent*>(event);
        window->GetGraphics().OnKeyDown(ev->GetKeyCode());

        switch (ev->GetKeyCode())
        {
        case VK_UP:
            if (tetromino == nullptr) break;
            if (!_isInputEnabled) break;
            toDrop = true;
            break;
        case VK_LEFT:
            if (tetromino == nullptr) break;
            if (!_isInputEnabled) break;
            toMoveLeft = true;
            break;
        case VK_DOWN:
            if (tetromino == nullptr) break;
            if (!_isInputEnabled) break;
            toMoveDown = true;
            break;
        case VK_RIGHT:
            if (tetromino == nullptr) break;
            if (!_isInputEnabled) break;
            toMoveRight = true;
            break;
        case 'Z':
            if (tetromino == nullptr) break;
            if (!_isInputEnabled) break;
            toRotateCounterClockwise = true;
            break;
        case 'X':
            if (tetromino == nullptr) break;
            if (!_isInputEnabled) break;
            toRotateClockwise = true;
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
    _progress = 0;
    tetromino = _board->SpawnTetromino(world, _meshPool, Levels[_currentLevel].color);
}

void TetrisApp::Reset()
{
    _progress = 0;
    tetromino = nullptr;
    _board->Clear(_meshPool);
}

void TetrisApp::OnTetrominoLanded()
{
    if (_board->HarvestTetromino(world, tetromino, _meshPool) == TetrisBoard::GameOver)
    {
        Logger::LogInfo("Game Over");
        Reset();
        return;
    }
    _progress += _board->ClearRowsIfNeeded(_meshPool);
    Tetromino::baseBuffer.progress = static_cast<float>(_progress) / static_cast<float>(Levels[_currentLevel].goal);
    if (_progress >= Levels[_currentLevel].goal)
    {
        Logger::LogInfo("Level passed, go to level #" + std::to_string(_currentLevel + 1));
        ++_currentLevel;
        Start();
        return;
    }
    tetromino = _board->SpawnTetromino(world, _meshPool, Levels[_currentLevel].color);
}

void TetrisApp::Update()
{
    App::Update();

    for (auto handler : _runOnNextTick)
    {
        handler();
        _runOnNextTick.clear();
    }

    if (tetromino == nullptr)
    {
        tetromino = _board->SpawnTetromino(world, _meshPool, Levels[_currentLevel].color);
    }

    if (toMoveLeft)
    {
        _board->MoveTetrominoLeft(tetromino);
        toMoveLeft = false;
    }

    if (toMoveRight)
    {
        _board->MoveTetrominoRight(tetromino);
        toMoveRight = false;
    }

    if (toRotateClockwise)
    {
        _board->RotateTetrominoClockwise(tetromino);
        toRotateClockwise = false;
    }

    if (toRotateCounterClockwise)
    {
        _board->RotateTetrominoCounterclockwise(tetromino);
        toRotateCounterClockwise = false;
    }

    if (toDrop)
    {
        _board->DropTetromino(tetromino);
        toDrop = false;
        return;
    }
    if (toMoveDown)
    {
        _board->MoveTetrominoDown(tetromino);
        toMoveDown = false;
    }

    _isInputEnabled = false;
    const float currentTime = Time::GetSecondsSinceFirstTick();
    if (currentTime - _lastTimeCheck > 1 / Levels[_currentLevel].speed)
    {
        _lastTimeCheck = currentTime;
        if (_board->MoveTetrominoDown(tetromino) == TetrisBoard::UnableToMove)
        {
            OnTetrominoLanded();
        }
    }
    _isInputEnabled = true;
}
