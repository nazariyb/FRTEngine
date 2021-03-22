#include "App.h"
#include "Utils/Logger/Logger.h"

namespace frt
{

App* App::_instance = nullptr;

App::App(int width, int height, const char* name)
    : _windowName(name), _windowWidth(width), _windowHeight(height)
{
    Logger::DebugLogInfo("Create App");
}

App::~App()
{
    delete window;
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

void App::Update()
{
    window->GetGraphics().Update();
    window->GetGraphics().Render();
}

void App::Init(HINSTANCE hInstance, HICON icon)
{
    Logger::LogInfo("Initing game app");
    window = new Window(_windowWidth, _windowHeight, _windowName, icon);
}

}
