#include "Tetromino.h"
#include <DirectXMath.h>
#include "Utils/Logger/Logger.h"
#include "TetrominoFactory.h"
#include "Render/MeshPool.h"
#include "App.h"

using namespace frt;
using namespace DirectX;

const unsigned int Tetromino::MeshesNum = 4;
Mesh::SceneObjectConstantBuffer Tetromino::baseBuffer{};

Tetromino::Tetromino(Type type)
    : Tetromino(type, 1.0f, {}, nullptr)
{}

Tetromino::Tetromino(Type type, float radius, XMFLOAT3 worldPosition, frt::MeshPool* meshPool/* = nullptr*/)
    : _type(type)
    , _radius(radius)
    , _rotation{}
    , _worldPosition{worldPosition}
    , _topBound{ std::numeric_limits<float>::min() }
    , _bottomBound{ std::numeric_limits<float>::max() }
    , _leftBound{ std::numeric_limits<float>::max() }
    , _rightBound{ std::numeric_limits<float>::min() }
{
    _owner = frt::App::GetInstance();

    if (meshPool == nullptr)
    {
        for (int i = 0; i < MeshesNum; ++i)
        {
            _meshes.push_back(new Mesh(_radius, { }));
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

    float innerOffsetX = 0.f, innerOffsetY = 0.f, innerOffsetZ = 0.f;
    const std::vector<XMFLOAT3>& offsets = TetrominoFactory::_offsets.at(_type);

    for (int i = 0; i < MeshesNum; ++i)
    {
        innerOffsetX = offsets[i].x;
        innerOffsetY = offsets[i].y;
        innerOffsetZ = offsets[i].z;

        if (innerOffsetX > _rightBound)
        {
            _rightBound = innerOffsetX;
        }
        else if (innerOffsetX < _leftBound)
        {
            _leftBound = innerOffsetX;
        }

        if (innerOffsetY > _topBound)
        {
            _topBound = innerOffsetY;
        }
        else if (innerOffsetY < _bottomBound)
        {
            _bottomBound = innerOffsetY;
        }
    }
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

    float top, bottom, left, right;
    if (rollDelta > 0.f)
    {
        top = _rightBound;
        right = -_bottomBound;
        bottom = _leftBound;
        left = -_topBound;
    }
    else
    {
        right = _topBound;
        bottom = -_rightBound;
        left = _bottomBound;
        top = -_leftBound;
    }

    _topBound = top;
    _rightBound = right;
    _bottomBound = bottom;
    _leftBound = left;
}

void Tetromino::UpdateConstantBuffers()
{
    float offsetX = 0.f, offsetY = 0.f, offsetZ = 0.f;
    XMFLOAT4X4 model, view, projection, mvp;

    const std::vector<XMFLOAT3>& offsets = TetrominoFactory::_offsets.at(_type);

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
        XMStoreFloat4x4(&projection, _owner->GetWindow()->GetGraphics()._camera.GetProjectionMatrix(0.8f, _owner->GetWindow()->GetGraphics()._aspectRatio));

        XMStoreFloat4x4(&mvp, XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection));

        XMStoreFloat4x4(&(buffer.mvp), XMMatrixTranspose(XMLoadFloat4x4(&mvp)));
        XMStoreFloat4x4(&(buffer.modelView), XMMatrixTranspose(XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view)));

        _meshes[i]->UpdateConstantBuffer(buffer);
    }
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
