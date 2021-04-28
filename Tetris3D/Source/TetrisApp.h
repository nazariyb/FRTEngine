#pragma once
#include "App.h"
#include <bitset>

using frt::App;

class TetrisApp :
    public App
{
public:
    TetrisApp();
    int Start() override;

protected:
    void Update() override;

    std::bitset<6> moveDirections;
    float Roll = 0.f;
    float Pitch = 0.f;
    float Yaw = 0.f;

    class Tetromino* object1;
};

