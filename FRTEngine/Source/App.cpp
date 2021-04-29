#include "App.h"
#include "Utils/Logger/Logger.h"
#include "Time/Time.h"

namespace frt
{

App* App::_instance = nullptr;

App::App(int width, int height, const char* name)
    : _windowName(name)
    , _windowWidth(width)
    , _windowHeight(height)
{
    Logger::DebugLogInfo("Create App");
    world = new GameWorld();
}

App::~App()
{
    delete window;
    delete world;
}

App* App::GetInstance()
{
    return _instance;
}

void App::Shutdown()
{
    delete _instance;
}

Window* App::GetWindow()
{
    return window;
}

GameWorld* App::GetWorld()
{
    return world;
}

void App::Update()
{
    Time::Tick();
    window->GetGraphics().Update();
    world->Update();
}

void App::Render()
{
    window->GetGraphics().Render();
}

void App::Init(HINSTANCE hInstance, HICON icon)
{
    Logger::LogInfo("Initting game app");
    window = new Window(_windowWidth, _windowHeight, _windowName, icon);
    Time::Init();
}

}
