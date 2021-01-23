#pragma once
#include "FRTEngine.h"
#include "Window.h"


using frt::Window;

namespace frt
{
    class FRTENGINE_API App
    {
    public:
        App() = default;
        App(const App&) = delete;
        App(int width, int height, const char* name, HICON icon);
        virtual ~App() {};

        virtual int Start() { return 0; };

    protected:
        virtual void Update();

        Window window;
    };
}

