#pragma once
#include "App.h"
#include <bitset>

using frt::App;
class Tetromino;
class TetrisBoard;

namespace frt
{
class MeshPool;
}

class TetrisApp :
    public App
{
public:
    TetrisApp();
    int Start() override;

protected:
    void Update() override;

    float Roll = 0.f;
    float Pitch = 0.f;
    float Yaw = 0.f;

    float _lastTimeCheck;

    Tetromino* object1;
    Tetromino* object2;

    frt::MeshPool* _meshPool;
    TetrisBoard* _board;
};

