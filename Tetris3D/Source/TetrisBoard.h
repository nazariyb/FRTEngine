#pragma once
#include "GameObject.h"
#include <DirectXMath.h>
#include <vector>

class Tetromino;
namespace frt
{
class Mesh;
class MeshPool;
class GameWorld;
}

class TetrisBoard : public frt::GameObject
{
public:
    TetrisBoard();
    TetrisBoard(unsigned int width, unsigned int height, float cellSize=2.0f);
    virtual ~TetrisBoard();

    Tetromino* SpawnTetromino(frt::GameWorld* gameWorld, frt::MeshPool* meshPool);
    void HarvestTetromino(frt::GameWorld* gameWorld, Tetromino* tetromino);
    
    inline void RotateTetrominoClockwise(Tetromino* tetromino)
        { RotateTetromino(tetromino, -DirectX::XM_PIDIV2); }
    inline void RotateTetrominoCounterclockwise(Tetromino* tetromino)
        { RotateTetromino(tetromino, DirectX::XM_PIDIV2); }

    inline bool MoveTetrominoLeft(Tetromino* tetromino)  { return MoveTetrominoLeft(tetromino, -_cellSize); }
    inline bool MoveTetrominoRight(Tetromino* tetromino) { return MoveTetrominoRight(tetromino, _cellSize); }
    inline bool MoveTetrominoDown(Tetromino* tetromino)  { return MoveTetrominoDown(tetromino, -_cellSize); }

    void DropTetromino(Tetromino* tetromino);

    unsigned ClearRowsIfNeeded(frt::MeshPool* meshPool);

    virtual void Update() override;

public:
    static DirectX::XMFLOAT3 TopBound;
    static DirectX::XMFLOAT3 BottomBound;
    static DirectX::XMFLOAT3 LeftBound;
    static DirectX::XMFLOAT3 RightBound;

private:
    unsigned int _width;
    unsigned int _height;
    float _cellSize;

    struct Cell
    {
        inline Cell(DirectX::XMFLOAT3 coordinates_, frt::Mesh* mesh_)
            : coordinates(coordinates_), mesh(mesh_)
        {}
        DirectX::XMFLOAT3 coordinates;
        frt::Mesh* mesh;
    };

    std::vector<Cell*> _cells;

    void UpdateContantBuffers();
    
    bool RotateTetromino(Tetromino* tetromino, float deltaRadians);
    
    bool MoveTetrominoLeft(Tetromino* tetromino, float deltaDistance);
    bool MoveTetrominoRight(Tetromino* tetromino, float deltaDistance);
    bool MoveTetrominoDown(Tetromino* tetromino, float deltaDistance, bool needCheck=true);
    
    bool IsMoveLeftPossible(Tetromino* tetromino);
    bool IsMoveRightPossible(Tetromino* tetromino);
    bool IsMoveDownPossible(Tetromino* tetromino);

    bool ArePositionsValid(const std::vector<DirectX::XMFLOAT3>& positions, const DirectX::XMFLOAT3& offset={});
    bool IsPositionValid(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& offset={});

    bool DoesTetrominoBlockCollideWithBoardBlock(const DirectX::XMFLOAT3& blockPosition);
};
