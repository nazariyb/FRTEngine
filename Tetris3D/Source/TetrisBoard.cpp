#include "TetrisBoard.h"

#include "App.h"
#include "Tetromino.h"
#include "Render/Mesh.h"
#include "Render/MeshPool.h"
#include "GameWorld.h"
#include <cstdlib>

using namespace frt;
using namespace DirectX;

XMFLOAT3 TetrisBoard::TopBound{0.f, 20.f, 0.f};
XMFLOAT3 TetrisBoard::BottomBound{0.f, 0.f, 0.f};
XMFLOAT3 TetrisBoard::LeftBound{-5.f, 10.f, 0.f};
XMFLOAT3 TetrisBoard::RightBound{5.f, 10.f, 0.f};

TetrisBoard::TetrisBoard()
    : TetrisBoard(10, 20)
{
}

TetrisBoard::TetrisBoard(unsigned int width, unsigned int height, float cellSize /* = 2.0f*/)
    : _width(width)
    , _height(height)
    , _cellSize(cellSize)
{
    _cells.reserve(_width * _height);

    int i = - static_cast<int>(_width / 2);
    const int iEnd = _width / 2;
    for (; i <= iEnd; ++i)
    {
        for (unsigned j = static_cast<unsigned>(_cellSize / 2);
            j <= _height * static_cast<unsigned>(_cellSize);
            j += static_cast<unsigned>(_cellSize))
        {
            _cells.push_back(new Cell({ static_cast<float>(i) * cellSize, static_cast<float>(j), 0 }, nullptr));
        }
    }

    XMStoreFloat3(&TetrisBoard::TopBound, XMVectorScale(XMLoadFloat3(&TetrisBoard::TopBound), _cellSize));
    XMStoreFloat3(&TetrisBoard::BottomBound, XMVectorScale(XMLoadFloat3(&TetrisBoard::BottomBound), _cellSize));
    XMStoreFloat3(&TetrisBoard::LeftBound, XMVectorScale(XMLoadFloat3(&TetrisBoard::LeftBound), _cellSize));
    XMStoreFloat3(&TetrisBoard::RightBound, XMVectorScale(XMLoadFloat3(&TetrisBoard::RightBound), _cellSize));
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
    return gameWorld->SpawnObject<Tetromino>(static_cast<Tetromino::Type>(rand() % 7), _cellSize / 2.f, TopBound, meshPool);
}

void TetrisBoard::HarvestTetromino(GameWorld* gameWorld, Tetromino* tetromino)
{
    for (int i = 0; i < tetromino->MeshesNum; ++i)
    {
        const XMFLOAT3& meshPosition = tetromino->_meshes[i]->GetWorldPosition();
        Logger::DebugLogInfo("try to harvest: <" +
            std::to_string(meshPosition.x) + ", " +
            std::to_string(meshPosition.y) + ", " +
            std::to_string(meshPosition.z) + ">");
        for (Cell* cell : _cells)
        {
            if (XMVector3NearEqual(XMLoadFloat3(&cell->coordinates),
                                   XMLoadFloat3(&meshPosition),
                                   XMVectorReplicate(1e-3)))
            {
                cell->mesh = tetromino->_meshes[i];
                tetromino->_meshes[i] = nullptr;
                Logger::DebugLogInfo("Harvested");
                break;
            }
        }
    }
    gameWorld->DestroyObject(tetromino);
    for (Cell* cell : _cells)
    {
        if (cell->mesh != nullptr)
        {
            Logger::DebugLogInfo("cell: <" +
                std::to_string(cell->coordinates.x) + ", " +
                std::to_string(cell->coordinates.y) + ", " +
                std::to_string(cell->coordinates.z) + ">");
        }
    }
}

void TetrisBoard::RotateTetrominoClockwise(Tetromino* tetromino)
{
    if (tetromino->GetLeftBoundAfterClockwiseRotation() < LeftBound.x)
    {
        // tetromino->MoveX(_cellSize);
    }
    else if (tetromino->GetRightBoundAfterClockwiseRotation() > RightBound.x)
    {
        // tetromino->MoveX(-_cellSize);
    }

    tetromino->RotateRoll(-XM_PIDIV2);
}

void TetrisBoard::RotateTetrominoCounterclockwise(Tetromino* tetromino)
{
    if (tetromino->GetRightBoundAfterCounterclockwiseRotation() > RightBound.x)
    {
        // tetromino->MoveX(-_cellSize);
    }
    else if (tetromino->GetLeftBoundAfterCounterclockwiseRotation() < LeftBound.x)
    {
        // tetromino->MoveX(_cellSize);
    }

    tetromino->RotateRoll(XM_PIDIV2);
}

bool TetrisBoard::MoveTetrominoLeft(Tetromino* tetromino)
{
    if (!IsMoveLeftPossible(tetromino)) return false;
        
    tetromino->MoveX(-_cellSize);
    return true;
}

bool TetrisBoard::MoveTetrominoRight(Tetromino* tetromino)
{
    if (!IsMoveRightPossible(tetromino)) return false;
    
    tetromino->MoveX(_cellSize);
    return true;
}

bool TetrisBoard::MoveTetrominoDown(Tetromino* tetromino)
{
    if (!IsMoveDownPossible(tetromino)) return false;
    
    tetromino->MoveY(-_cellSize);
    return true;
}

void TetrisBoard::DropTetromino(Tetromino* tetromino)
{
}

void TetrisBoard::Update()
{
}

bool TetrisBoard::IsMoveLeftPossible(Tetromino* tetromino)
{
    if (tetromino->GetLeftBound() <= LeftBound.x + 1e-3) return false;

    XMFLOAT3 meshPosition;
    for (unsigned int i = 0; i < tetromino->MeshesNum; ++i)
    {
        meshPosition = tetromino->_meshes[i]->GetWorldPosition();
        meshPosition.x -= _cellSize;
        for (Cell* cell : _cells)
        {
            if (cell->mesh != nullptr && XMVector3NearEqual(
                       XMLoadFloat3(&cell->coordinates),
                       XMLoadFloat3(&meshPosition),
                       XMVectorReplicate(1e-3)))
                           return false;
        }
    }
    return true;
}

bool TetrisBoard::IsMoveRightPossible(Tetromino* tetromino)
{
    if (tetromino->GetRightBound() >= RightBound.x - 1e-3) return false;

    XMFLOAT3 meshPosition;
    for (unsigned int i = 0; i < tetromino->MeshesNum; ++i)
    {
        meshPosition = tetromino->_meshes[i]->GetWorldPosition();
        meshPosition.x += _cellSize;
        for (Cell* cell : _cells)
        {
            if (cell->mesh != nullptr && XMVector3NearEqual(
                       XMLoadFloat3(&cell->coordinates),
                       XMLoadFloat3(&meshPosition),
                       XMVectorReplicate(1e-3)))
                           return false;
        }
    }
    return true;
}

bool TetrisBoard::IsMoveDownPossible(Tetromino* tetromino)
{
    if (tetromino->GetBottomBound() <= BottomBound.y + 1e-3) return false;

    XMFLOAT3 meshPosition;
    for (unsigned int i = 0; i < tetromino->MeshesNum; ++i)
    {
        meshPosition = tetromino->_meshes[i]->GetWorldPosition();
        meshPosition.y -= _cellSize;
        for (Cell* cell : _cells)
        {
            if (cell->mesh != nullptr && XMVector3NearEqual(
                       XMLoadFloat3(&cell->coordinates),
                       XMLoadFloat3(&meshPosition),
                       XMVectorReplicate(1e-3)))
                           return false;
        }
    }
    return true;

}
