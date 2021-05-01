#include "TetrisBoard.h"
#include "Tetromino.h"
#include "Render/Mesh.h"
#include "Render/MeshPool.h"
#include "GameWorld.h"

using namespace frt;
using namespace DirectX;

XMFLOAT3 TetrisBoard::TopBound    {  0.f, 20.f, 0.f };
XMFLOAT3 TetrisBoard::BottomBound {  0.f,  0.f, 0.f };
XMFLOAT3 TetrisBoard::LeftBound   { -5.f, 10.f, 0.f };
XMFLOAT3 TetrisBoard::RightBound  {  5.f, 10.f, 0.f };

TetrisBoard::TetrisBoard()
    : TetrisBoard(10, 20)
{}

TetrisBoard::TetrisBoard(unsigned int width, unsigned int height, float cellSize /* = 2.0f*/)
    : _width(width)
    , _height(height)
    , _cellSize(cellSize)
{
    _cells.reserve(_width * _height);

    for (unsigned int i = 0; i < _width; ++i)
    {
        for (unsigned int j = 0; j < _height; ++j)
        {
            _cells.push_back(new Cell( {}, nullptr ));
        }
    }

    XMStoreFloat3(&TetrisBoard::TopBound,    XMVectorScale(XMLoadFloat3(&TetrisBoard::TopBound),    _cellSize));
    XMStoreFloat3(&TetrisBoard::BottomBound, XMVectorScale(XMLoadFloat3(&TetrisBoard::BottomBound), _cellSize));
    XMStoreFloat3(&TetrisBoard::LeftBound,   XMVectorScale(XMLoadFloat3(&TetrisBoard::LeftBound),   _cellSize));
    XMStoreFloat3(&TetrisBoard::RightBound,  XMVectorScale(XMLoadFloat3(&TetrisBoard::RightBound),  _cellSize));
}

TetrisBoard::~TetrisBoard()
{
    for (unsigned int i = 0; i < _cells.size(); ++i)
    {
        delete _cells.at(i);
    }
}

Tetromino* TetrisBoard::SpawnTetromino(GameWorld* gameWorld, MeshPool* meshPool)
{
    return gameWorld->SpawnObject<Tetromino>(Tetromino::Type::T, _cellSize / 2.f, TopBound, meshPool);
}

void TetrisBoard::RotateTetrominoClockwise(Tetromino* tetromino)
{
    if (tetromino->GetLeftBoundAfterClockwiseRotation() < LeftBound.x)
    {
        tetromino->MoveX(_cellSize);
    }
    else if (tetromino->GetRightBoundAfterClockwiseRotation() > RightBound.x)
    {
        tetromino->MoveX(-_cellSize);
    }

    tetromino->RotateRoll(-XM_PIDIV2);
}

void TetrisBoard::RotateTetrominoCounterclockwise(Tetromino* tetromino)
{
    if (tetromino->GetRightBoundAfterCounterclockwiseRotation() > RightBound.x)
    {
        tetromino->MoveX(-_cellSize);
    }
    else if (tetromino->GetLeftBoundAfterCounterclockwiseRotation() < LeftBound.x)
    {
        tetromino->MoveX(_cellSize);
    }

    tetromino->RotateRoll(XM_PIDIV2);
}

bool TetrisBoard::MoveTetrominoLeft(Tetromino* tetromino)
{
    if (tetromino->GetLeftBound() > LeftBound.x)
    {
        tetromino->MoveX(-_cellSize);
        return true;
    }
    return false;
}

bool TetrisBoard::MoveTetrominoRight(Tetromino* tetromino)
{
    if (tetromino->GetRightBound() < RightBound.x)
    {
        tetromino->MoveX(_cellSize);
        return true;
    }
    return false;
}

bool TetrisBoard::MoveTetrominoDown(Tetromino* tetromino)
{
    if (tetromino->GetBottomBound() > BottomBound.y)
    {
        tetromino->MoveY(-_cellSize);
        return true;
    }
    return false;
}

void TetrisBoard::DropTetromino(Tetromino* tetromino)
{

}

void TetrisBoard::Update()
{}
