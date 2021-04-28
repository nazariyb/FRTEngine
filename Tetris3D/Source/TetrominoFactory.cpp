#include "TetrominoFactory.h"

Tetromino* TetrominoFactory::CreateTetromino(Tetromino::Type type)
{
    // TODO:
    return nullptr;
}

const std::unordered_map<Tetromino::Type, std::vector<DirectX::XMFLOAT3>> TetrominoFactory::_offsets =
{
    {
        Tetromino::Type::I,
        {
            { -3.f, 1.f, 0.f },
            { -1.f, 1.f, 0.f },
            {  1.f, 1.f, 0.f },
            {  3.f, 1.f, 0.f }
        }
    },
    {
        Tetromino::Type::O,
        {
            { -1.f,  1.f, 0.f },
            {  1.f,  1.f, 0.f },
            {  1.f, -1.f, 0.f },
            { -1.f, -1.f, 0.f }
        }
    },
    {
        Tetromino::Type::T,
        {
            {  0.f, 2.f, 0.f },
            { -2.f, 0.f, 0.f },
            {  0.f, 0.f, 0.f },
            {  2.f, 0.f, 0.f }
        }
    },
    {
        Tetromino::Type::J,
        {
            { -2.f, 2.f, 0.f },
            { -2.f, 0.f, 0.f },
            {  0.f, 0.f, 0.f },
            {  2.f, 0.f, 0.f }
        }
    },
    {
        Tetromino::Type::L,
        {
            { -2.f, 2.f, 0.f },
            { -2.f, 0.f, 0.f },
            {  0.f, 0.f, 0.f },
            {  2.f, 0.f, 0.f }
        }
    },
    {
        Tetromino::Type::S,
        {
            {  0.f, 2.f, 0.f },
            {  2.f, 2.f, 0.f },
            { -2.f, 0.f, 0.f },
            {  0.f, 0.f, 0.f }
        }
    },
    {
        Tetromino::Type::Z,
        {
            { -2.f, 2.f, 0.f },
            {  0.f, 2.f, 0.f },
            {  0.f, 0.f, 0.f },
            {  2.f, 0.f, 0.f }
        }
    }
};
