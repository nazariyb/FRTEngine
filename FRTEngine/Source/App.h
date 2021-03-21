#pragma once
#include "FRTEngine.h"
#include "WindowsMinimal.h"
#include "Window.h"

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
    static frt::App* Launch(HINSTANCE hInstance, HICON icon);
    static void Close();

    Window* GetWindow();

protected:
    virtual int Start() { return 0; };

    void Init(HINSTANCE hInstance, HICON icon);

    virtual void Update();

    Window* window;

private:
    static App* _instance;

    int _windowWidth, _windowHeight;
    const char* _windowName;
};


template<class T>
frt::App* frt::App::Launch(HINSTANCE hInstance, HICON icon)
{
    static_assert((std::is_base_of<frt::App, T>::value));
    _instance = new T();
    _instance->Init(hInstance, icon);
    _instance->Start();
    return _instance;
}

}

