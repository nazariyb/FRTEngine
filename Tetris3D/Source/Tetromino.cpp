#include "Tetromino.h"
#include <DirectXMath.h>
#include "Utils/Logger/Logger.h"
#include "TetrominoFactory.h"
#include "Render/MeshPool.h"
#include "App.h"
#include "Time/Time.h"

using namespace frt;
using namespace DirectX;

const unsigned int Tetromino::MeshesNum = 4;
Mesh::SceneObjectConstantBuffer Tetromino::baseBuffer{};

Tetromino::Tetromino(Type type)
    : Tetromino(type, 1.0f, {}, nullptr)
{
}

Tetromino::Tetromino(Type type, float radius, XMFLOAT3 worldPosition, frt::MeshPool* meshPool/* = nullptr*/)
    : _type(type)
      , _radius(radius)
      , _rotation{}
      , _worldPosition{worldPosition}
      , _topBound{-std::numeric_limits<float>::infinity()}
      , _bottomBound{std::numeric_limits<float>::infinity()}
      , _leftBound{std::numeric_limits<float>::infinity()}
      , _rightBound{-std::numeric_limits<float>::infinity()}
{
    _owner = frt::App::GetInstance();

    if (meshPool == nullptr)
    {
        for (int i = 0; i < MeshesNum; ++i)
        {
            _meshes.push_back(new Mesh(_radius, {}));
        }
    }
    else
    {
        auto getResult = meshPool->GetFreeMeshes(MeshesNum);
        _meshes = getResult.second;
        for (Mesh* mesh : _meshes)
        {
            mesh->Resize(_radius);
        }
    }

    const std::vector<XMFLOAT3>& offsets = TetrominoFactory::_offsets.at(_type);

    if (static_cast<int>(offsets[0].x) % 2)
        _worldPosition.x -= _radius;

    if (static_cast<int>(offsets[0].y) % 2)
        _worldPosition.y -= _radius;
}

void Tetromino::RotatePitch(float pitchDelta)
{
    _rotation.x += pitchDelta;
}

void Tetromino::RotateYaw(float yawDelta)
{
    _rotation.z += yawDelta;
}

void Tetromino::RotateRoll(float rollDelta)
{
    _rotation.y += rollDelta;
}

Tetromino::BodyState Tetromino::GetBoundsAfterRotation(float deltaRadians)
{
    std::vector<XMFLOAT3> newPositions;
    newPositions.resize(MeshesNum);
    
    const std::vector<XMFLOAT3>& offsets = TetrominoFactory::_offsets.at(_type);

    Bounds bounds{
        { std::numeric_limits<float>::infinity(), 0.f },
        { 0.f, -std::numeric_limits<float>::infinity() },
        { -std::numeric_limits<float>::infinity(), 0.f },
        { 0.f, std::numeric_limits<float>::infinity() },
    };
   
    for (int i = 0; i < MeshesNum; ++i)
    {
        XMStoreFloat3(&newPositions[i],
                      XMVector3Transform(XMLoadFloat3(&offsets[i]),
                                         XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.z, _rotation.y + deltaRadians)));
        newPositions[i].x /= _radius;
        newPositions[i].y /= _radius;
        newPositions[i].y += _radius;
        XMStoreFloat3(&newPositions[i],
                      XMVectorAdd(XMLoadFloat3(&newPositions[i]), XMLoadFloat3(&_worldPosition)));

        if (newPositions[i].x < bounds.left.x)
        {
            bounds.left.x = newPositions[i].x;
            bounds.left.y = newPositions[i].y;
        }
        if (newPositions[i].x > bounds.right.x)
        {
            bounds.right.x = newPositions[i].x;
            bounds.right.y = newPositions[i].y;
        }
        if (newPositions[i].y > bounds.top.y)
        {
            bounds.top.y = newPositions[i].y;
            bounds.top.x = newPositions[i].x;
        }
        if (newPositions[i].y < bounds.bottom.y)
        {
            bounds.bottom.y = newPositions[i].y;
            bounds.bottom.x = newPositions[i].x;
        }
    }
    bounds.left.x -= _radius;
    bounds.top.y += _radius;
    bounds.right.x += _radius;
    bounds.bottom.y -= _radius;
    return { bounds, newPositions };
}

void Tetromino::UpdateConstantBuffers()
{
    float offsetX = 0.f, offsetY = 0.f, offsetZ = 0.f;
    XMFLOAT4X4 model, view, projection, mvp;

    const std::vector<XMFLOAT3>& offsets = TetrominoFactory::_offsets.at(_type);

    XMFLOAT4 bounds{
        std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
    };
   
    for (int i = 0; i < MeshesNum; ++i)
    {
        offsetX = _radius * offsets[i].x;
        offsetY = _radius * offsets[i].y;
        offsetZ = _radius * offsets[i].z;

        Mesh::SceneObjectConstantBuffer buffer;
        memcpy(&buffer, &baseBuffer, sizeof(Mesh::SceneObjectConstantBuffer));

        XMStoreFloat4x4(&model, XMMatrixMultiply(
                            XMMatrixMultiply(XMMatrixTranslation(offsetX, offsetY, offsetZ),
                                             XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.z, _rotation.y)),
                            XMMatrixTranslation(_worldPosition.x, _worldPosition.y, _worldPosition.z)));
        
        XMStoreFloat4x4(&view, _owner->GetWindow()->GetGraphics()._camera.GetViewMatrix());
        XMStoreFloat4x4(&projection,
                        _owner->GetWindow()->GetGraphics()._camera.GetProjectionMatrix(
                            1.f, _owner->GetWindow()->GetGraphics()._aspectRatio));

        XMStoreFloat4x4(&mvp, XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection));

        // XMStoreFloat4x4(&buffer.mvp, XMMatrixTranspose(XMLoadFloat4x4(&mvp)));
        // XMStoreFloat4x4(&buffer.modelView, XMMatrixTranspose(XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view)));
        XMStoreFloat4x4(&buffer.model, XMMatrixTranspose(XMLoadFloat4x4(&model)));
        XMStoreFloat4x4(&buffer.viewProj, XMMatrixTranspose(XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection)));

        XMStoreFloat3(&_meshes[i]->GetWorldPosition(),
                      XMVector3Transform(XMLoadFloat3(&offsets[i]),
                                         XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.z, _rotation.y)));
        _meshes[i]->GetWorldPosition().x /= _radius;
        _meshes[i]->GetWorldPosition().y /= _radius;
        _meshes[i]->GetWorldPosition().y += _radius;
        XMStoreFloat3(&_meshes[i]->GetWorldPosition(),
                      XMVectorAdd(XMLoadFloat3(&_meshes[i]->GetWorldPosition()), XMLoadFloat3(&_worldPosition)));

        const XMFLOAT3& meshPosition = _meshes[i]->GetWorldPosition();

        if (meshPosition.x < bounds.x)
        {
            bounds.x = meshPosition.x;
        }
        if (meshPosition.x > bounds.z)
        {
            bounds.z = meshPosition.x;
        }
        if (meshPosition.y > bounds.y)
        {
            bounds.y = meshPosition.y;
        }
        if (meshPosition.y < bounds.w)
        {
            bounds.w = meshPosition.y;
        }
        
        _meshes[i]->UpdateConstantBuffer(buffer);
    }
    _leftBound = bounds.x - _radius;
    _topBound = bounds.y + _radius;
    _rightBound = bounds.z + _radius;
    _bottomBound = bounds.w - _radius;

    // Logger::DebugLogInfo("u bound ", {_leftBound, _topBound, _rightBound, _bottomBound});
}

void Tetromino::Update()
{
    UpdateConstantBuffers();
}

void Tetromino::PopulateCommandList()
{
    //for (Mesh* mesh : _meshes)
    //{
    //    mesh->PopulateCommandList();
    //}
}

void Tetromino::InitializeGraphicsResources(Graphics* graphics)
{
    //for (Mesh* mesh : _meshes)
    //{
    //    mesh->InitializeGraphicsResources(graphics);
    //}
}

void Tetromino::InitializeConstantBuffers(Graphics* graphics)
{
    //for (Mesh* mesh : _meshes)
    //{
    //    mesh->InitializeConstantBuffers(graphics);
    //}
}
