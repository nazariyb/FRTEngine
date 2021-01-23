#include "App.h"


frt::App::App(int width, int height, const char* name, HICON icon)
    :window(width, height, name, icon)
{
}

void frt::App::Update()
{
    window.GetGraphics().Update();
    window.GetGraphics().Render();
}
