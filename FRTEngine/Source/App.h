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

    virtual int Start() { return 0; };

    void Init(HINSTANCE hInstance, HICON icon);
    
    template<class T>
    static frt::App* CreateGameApp();

protected:
    virtual void Update();

    Window* window;

private:
    int _windowWidth, _windowHeight;
    const char* _windowName;
};


template<class T>
frt::App* frt::App::CreateGameApp()
{
    assert((std::is_base_of<frt::App, T>::value));
    return new T();
}

}

