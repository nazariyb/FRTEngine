#include "Tetrimino.h"
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
