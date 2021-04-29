#pragma once
#include "Tetrimino.h"
#include <unordered_map>
#include <vector>
#include <DirectXMath.h>


class TetrominoFactory
{
public:
    static Tetromino* CreateTetromino(Tetromino::Type type);

public:
    static const std::unordered_map<Tetromino::Type, std::vector<DirectX::XMFLOAT3>> _offsets;
};

