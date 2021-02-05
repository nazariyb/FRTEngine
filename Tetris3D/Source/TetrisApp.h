#pragma once
#include "App.h"


using frt::App;

class TetrisApp :
    public App
{
public:
    TetrisApp();
    int Start() override;

protected:
    void Update() override;
};

