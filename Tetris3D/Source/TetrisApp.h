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
    struct LevelInfo
    {
        DirectX::XMFLOAT4 color;
        float speed;
        unsigned goal;
    };
    static const LevelInfo Levels[10];
    
public:
    TetrisApp();
    virtual ~TetrisApp();
    
    int Run() override;

protected:
    void Start();
    void Reset();
    
    void Update() override;

    void OnTetrominoLanded();

    unsigned _currentLevel;
    unsigned _progress;
    float _lastTimeCheck;
    bool _isInputEnabled;
    bool toDrop, toMoveDown;
    
    Tetromino* tetromino;

    frt::MeshPool* _meshPool;
    TetrisBoard* _board;

    std::vector<std::function<void()>> _runOnNextTick;
};

