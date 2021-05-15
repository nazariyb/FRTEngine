#pragma once
#include "FRTEngine.h"
#include "Window.h"
#include "GameWorld.h"
#include <cassert>

using frt::Window;

namespace frt
{
class FRTENGINE_API App
{
public:
    App() = default;
    App(const App&) = delete;
    App(int width, int height, const char* name);
    virtual ~App();

    static App* GetInstance();
    
    template<class T>
    static App* Launch(HINSTANCE hInstance, HICON icon);
    static void Shutdown();

    Window* GetWindow();
    GameWorld* GetWorld();

    struct { const unsigned width; const unsigned height; } GetResolution() const { return { _windowWidth, _windowHeight }; }

protected:
    virtual int Run() = 0;

    void Init(HINSTANCE hInstance, HICON icon);

    virtual void Update();
    virtual void Render();

    Window* window;
    GameWorld* world;

    static App* _instance;

    unsigned _windowWidth, _windowHeight;
    const char* _windowName;
};


template<class T>
App* App::Launch(HINSTANCE hInstance, HICON icon)
{
    static_assert(std::is_base_of<App, T>::value);
    _instance = new T();
    _instance->Init(hInstance, icon);
    _instance->Run();
    return _instance;
}

}
