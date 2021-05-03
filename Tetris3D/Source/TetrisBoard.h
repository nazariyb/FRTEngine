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
    ~TetrisBoard();

    Tetromino* SpawnTetromino(frt::GameWorld* gameWorld, frt::MeshPool* meshPool);
    void HarvestTetromino(frt::GameWorld* gameWorld, Tetromino* tetromino);
    
    void RotateTetrominoClockwise(Tetromino* tetromino);
    void RotateTetrominoCounterclockwise(Tetromino* tetromino);
    
    bool MoveTetrominoLeft(Tetromino* tetromino);
    bool MoveTetrominoRight(Tetromino* tetromino);
    bool MoveTetrominoDown(Tetromino* tetromino);

    void DropTetromino(Tetromino* tetromino);


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
};
