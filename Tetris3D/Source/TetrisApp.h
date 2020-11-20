#pragma once
#include "App.h"


using frt::App;

class TetrisApp :
    public App
{
public:
    TetrisApp(HINSTANCE hInstance);
    int Start() override;

protected:
    void Update() override;
};

