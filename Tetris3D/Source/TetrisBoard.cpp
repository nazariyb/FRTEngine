#include "TetrisBoard.h"

#include <complex>
#include "App.h"
#include "Tetromino.h"
#include "Render/Mesh.h"
#include "Render/MeshPool.h"
#include "GameWorld.h"
#include <cstdlib>
#include <ctime>

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

    const int iEnd = _width / 2;
    for (unsigned j = static_cast<unsigned>(_cellSize / 2);
         j <= _height * static_cast<unsigned>(_cellSize);
         j += static_cast<unsigned>(_cellSize))
    {
        for (int i = -iEnd; i <= iEnd; ++i)
        {
            _cells.push_back(new Cell({static_cast<float>(i) * cellSize, static_cast<float>(j), 0}, nullptr));
        }
    }

    XMStoreFloat3(&TetrisBoard::TopBound, XMVectorScale(XMLoadFloat3(&TetrisBoard::TopBound), _cellSize));
    XMStoreFloat3(&TetrisBoard::BottomBound, XMVectorScale(XMLoadFloat3(&TetrisBoard::BottomBound), _cellSize));
    XMStoreFloat3(&TetrisBoard::LeftBound, XMVectorScale(XMLoadFloat3(&TetrisBoard::LeftBound), _cellSize));
    XMStoreFloat3(&TetrisBoard::RightBound, XMVectorScale(XMLoadFloat3(&TetrisBoard::RightBound), _cellSize));

    std::srand(std::time(nullptr));
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
    // return gameWorld->SpawnObject<Tetromino>(Tetromino::Type::T, _cellSize / 2.f, TopBound, meshPool);
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

void TetrisBoard::UpdateContantBuffers()
{
    XMFLOAT4X4 model, view, projection, mvp;
    XMFLOAT3 meshPosition;
    Mesh::SceneObjectConstantBuffer buffer;

    for (Cell* cell : _cells)
    {
        if (cell->mesh == nullptr) continue;

        meshPosition = cell->mesh->GetWorldPosition();

        XMStoreFloat4x4(&model, XMMatrixTranslation(meshPosition.x, meshPosition.y - _cellSize / 2.f, meshPosition.z));
        XMStoreFloat4x4(&view, App::GetInstance()->GetWindow()->GetGraphics()._camera.GetViewMatrix());
        XMStoreFloat4x4(&projection,
                        App::GetInstance()->GetWindow()->GetGraphics()._camera.GetProjectionMatrix(
                            1.f, App::GetInstance()->GetWindow()->GetGraphics()._aspectRatio));
        
        XMStoreFloat4x4(&mvp, XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection));

        memcpy(&buffer, &Tetromino::baseBuffer, sizeof(Mesh::SceneObjectConstantBuffer));
        // XMStoreFloat4x4(&buffer.mvp, XMMatrixTranspose(XMLoadFloat4x4(&mvp)));
        // XMStoreFloat4x4(&buffer.modelView, XMMatrixTranspose(XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view)));
        XMStoreFloat4x4(&buffer.model, XMMatrixTranspose(XMLoadFloat4x4(&model)));
        XMStoreFloat4x4(&buffer.viewProj, XMMatrixTranspose(XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection)));
        
        cell->mesh->UpdateConstantBuffer(buffer);
    }
}

bool TetrisBoard::RotateTetromino(Tetromino* tetromino, float deltaRadians)
{
    const Tetromino::BodyState& newState = tetromino->GetBoundsAfterRotation(deltaRadians);
    const Tetromino::Bounds newBounds = newState.bounds;
    if (newBounds.bottom.y < BottomBound.y - _cellSize / 2.)
    {
        tetromino->MoveY(BottomBound.y - newBounds.bottom.y);
    }
    else if (newBounds.left.x < LeftBound.x - _cellSize / 2.)
    {
        if (!ArePositionsValid(newState.meshPositions, {LeftBound.x - _cellSize / 2.f - newBounds.left.x, 0.f, 0.f}))
            return false;
        if (!MoveTetrominoRight(tetromino, LeftBound.x - _cellSize / 2. - newBounds.left.x)) return false;
    }
    else if (newBounds.right.x > RightBound.x + _cellSize / 2.)
    {
        if (!ArePositionsValid(newState.meshPositions, {
                                   RightBound.x + _cellSize / 2.f - newBounds.right.x, 0.f, 0.f
                               }))
            return false;
        if (!MoveTetrominoLeft(tetromino, RightBound.x + _cellSize / 2.f - newBounds.right.x)) return false;
    }
    else
    {
        enum
        {
            noneD,
            leftD,
            topD,
            rightD
        } directionToMove = noneD;

        float distanceToMove = 0.f;

        for (const XMFLOAT3& newPosition : newState.meshPositions)
        {
            if (!IsPositionValid(newPosition))
            {
                float X{}, Y{};
                for (const XMFLOAT3& newPosition_ : newState.meshPositions)
                {
                    X += newPosition_.x;
                    Y += newPosition_.y;
                }

                XMFLOAT3 center
                {
                    X / newState.meshPositions.size(),
                    Y / newState.meshPositions.size(),
                    0.f
                };

                XMFLOAT3 direction;
                XMStoreFloat3(&direction,
                              XMVector3Normalize(XMVectorSubtract(
                                  XMLoadFloat3(&newPosition), XMLoadFloat3(&center)
                              ))
                );

                XMFLOAT3 right{1.f, 0.f, 0.f};
                XMFLOAT3 bottom{0.f, -1.0f, 0.f};
                XMFLOAT3 left{-1.f, 0.f, 0.f};
                const float angleRight = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&direction), XMLoadFloat3(&right)));
                const float angleBottom = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&direction), XMLoadFloat3(&bottom)));
                const float angleLeft = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&direction), XMLoadFloat3(&left)));

                Logger::DebugLogInfo("direction: ", {direction.x, direction.y, direction.z, 0.f});

                float distToBound;
                if (angleRight > angleLeft && angleRight > angleBottom)
                {
                    directionToMove = leftD;
                    distToBound = newPosition.x - newBounds.left.x + tetromino->_radius;
                    Logger::DebugLogInfo("it's right");
                }
                else if (angleLeft > angleRight && angleLeft > angleBottom)
                {
                    directionToMove = rightD;
                    distToBound = newBounds.right.x - newPosition.x - tetromino->_radius;
                    Logger::DebugLogInfo("it's left");
                }
                else
                {
                    directionToMove = topD;
                    distToBound = newPosition.y - newBounds.bottom.y + tetromino->_radius;
                    Logger::DebugLogInfo("it's bottom");
                }

                if (distToBound > distanceToMove)
                {
                    distanceToMove = distToBound;
                }
            }
        }

        if (directionToMove == leftD)
        {
            if (!ArePositionsValid(newState.meshPositions, {-distanceToMove, 0.f, 0.f})) return false;
            if (!MoveTetrominoLeft(tetromino, -distanceToMove)) return false;
        }
        else if (directionToMove == topD)
        {
            if (!ArePositionsValid(newState.meshPositions, {0.f, distanceToMove, 0.f})) return false;
            tetromino->MoveY(distanceToMove);
        }
        else if (directionToMove == rightD)
        {
            if (!ArePositionsValid(newState.meshPositions, {distanceToMove, 0.f, 0.f})) return false;
            if (!MoveTetrominoRight(tetromino, distanceToMove)) return false;
        }
    }

    tetromino->RotateRoll(deltaRadians);
    return true;
}

bool TetrisBoard::MoveTetrominoLeft(Tetromino* tetromino, float deltaDistance)
{
    if (!IsMoveLeftPossible(tetromino)) return false;

    tetromino->MoveX(deltaDistance);
    return true;
}

bool TetrisBoard::MoveTetrominoRight(Tetromino* tetromino, float deltaDistance)
{
    if (!IsMoveRightPossible(tetromino)) return false;

    tetromino->MoveX(deltaDistance);
    return true;
}

bool TetrisBoard::MoveTetrominoDown(Tetromino* tetromino, float deltaDistance, bool needCheck/*=true*/)
{
    if (needCheck) if (!IsMoveDownPossible(tetromino)) return false;

    tetromino->MoveY(deltaDistance);
    return true;
}

void TetrisBoard::DropTetromino(Tetromino* tetromino)
{
    std::vector<float> dists;
    XMFLOAT3 meshPosition;
    int row;
    unsigned column;
    Cell* cell;
    
    for (Mesh* cube : tetromino->_meshes)
    {
        meshPosition = cube->GetWorldPosition();
        dists.push_back(meshPosition.y);
        
        row = static_cast<int>((meshPosition.y - 1) / _cellSize);
        column = static_cast<unsigned>(meshPosition.x / _cellSize) + _width / 2;
        
        for (; row >= 0; --row)
        {
            if (row >= _height) continue;
            cell = _cells[row * (_width + 1) + column];
            if (cell->mesh != nullptr)
            {
                float dist = meshPosition.y - (cell->coordinates.y + _cellSize / 2.);
                if (dist < dists.back())
                {
                    dists.at(dists.size() - 1) = dist;
                }
                break;
            }
        }
    }
    const float distToMove = *std::min_element(dists.begin(), dists.end()) - _cellSize / 2;
    MoveTetrominoDown(tetromino, -distToMove, false);
}

unsigned TetrisBoard::ClearRowsIfNeeded(MeshPool* meshPool)
{
    std::vector<unsigned> rowsToClear;
    bool rowNeedBeCleared;
    for (unsigned i = 0; i < _height; ++i)
    {
        rowNeedBeCleared = true;
        for (unsigned j = 0; j <= _width; ++j)
        {
            if (_cells[i * (_width + 1) + j]->mesh == nullptr)
            {
                rowNeedBeCleared = false;
                break;
            }
        }
        if (rowNeedBeCleared) rowsToClear.push_back(i);
    }

    {
        Cell* cell;
        for (unsigned rowToClear : rowsToClear)
        {
            for (unsigned i = 0; i <= _width; ++i)
            {
                cell = _cells[rowToClear * (_width + 1) + i];
                cell->mesh->Resize(0.f);
                cell->mesh->UpdateConstantBuffer({});
                meshPool->ReleaseMesh(cell->mesh);
                cell->mesh = nullptr;
            }
            Logger::DebugLogInfo("Row #" + std::to_string(rowToClear) + " cleared");
        }
    }

    if (rowsToClear.size() == 0) return 0;

    unsigned currentRow = rowsToClear.front() + 1;

    for (; currentRow < _height; ++currentRow)
    {
        unsigned cellsToMove = 0;
        for (unsigned rowToClear : rowsToClear)
        {
            if (rowToClear < currentRow)
            {
                ++cellsToMove;
            }
            else
            {
                break;
            }
        }
        for (unsigned j = 0; j <= _width; ++j)
        {
            Cell* cell = _cells[currentRow * (_width + 1) + j];
            if (cell->mesh == nullptr) continue;
            XMFLOAT3& currentPosition = cell->mesh->GetWorldPosition();
            currentPosition.y -= cellsToMove * _cellSize;
            _cells[(currentRow - 1) * (_width + 1) + j]->mesh = cell->mesh;
            cell->mesh = nullptr;
        }
    }
    
    return rowsToClear.size();
}

void TetrisBoard::Update()
{
    UpdateContantBuffers();
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

bool TetrisBoard::ArePositionsValid(const std::vector<XMFLOAT3>& positions, const XMFLOAT3& offset/*={}*/)
{
    XMFLOAT3 meshPosition;
    for (unsigned int i = 0; i < positions.size(); ++i)
    {
        XMStoreFloat3(&meshPosition, XMVectorAdd(XMLoadFloat3(&positions[i]), XMLoadFloat3(&offset)));

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

bool TetrisBoard::IsPositionValid(const XMFLOAT3& position, const XMFLOAT3& offset)
{
    XMFLOAT3 meshPosition;
    XMStoreFloat3(&meshPosition, XMVectorAdd(XMLoadFloat3(&position), XMLoadFloat3(&offset)));

    for (Cell* cell : _cells)
    {
        if (cell->mesh != nullptr && XMVector3NearEqual(
            XMLoadFloat3(&cell->coordinates),
            XMLoadFloat3(&meshPosition),
            XMVectorReplicate(1e-3)))
            return false;
    }
    return true;
}

bool TetrisBoard::DoesTetrominoBlockCollideWithBoardBlock(const XMFLOAT3& blockPosition)
{
    for (Cell* cell : _cells)
    {
        if (cell->mesh != nullptr && XMVector3NearEqual(
            XMLoadFloat3(&cell->coordinates),
            XMLoadFloat3(&blockPosition),
            XMVectorReplicate(1e-3)))
            return true;
    }
    return false;
}
