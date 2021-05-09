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
    int Run() override;

protected:
    void OnTetrominoLanded();
    
    void Update() override;

    float _lastTimeCheck;

    Tetromino* tetromino;

    frt::MeshPool* _meshPool;
    TetrisBoard* _board;

    std::function<void()> _runOnNextTick;
};

