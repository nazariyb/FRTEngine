#include "Tetrimino.h"
#include <DirectXMath.h>
#include "Utils/Logger/Logger.h"

using namespace frt;
using namespace DirectX;


Tetromino::Tetromino()
    : _pitch(0.0f)
    , _yaw(0.0f)
    , _roll(0.0f)
{
    _owner = frt::App::GetInstance();

    float radius = 1.0f;
    float offsetX = 0.f, offsetY = 0.f;
    XMFLOAT4X4 model, view, projection, mvp;

    for (int i = 0; i < 4; ++i)
    {
        offsetX = std::pow(-1.f, 1 * i) * -1.f;
        offsetY = std::pow(-1.f, i / 2);

        Logger::DebugLogInfo("Spawn cube at " + std::to_string(offsetX) + ", " + std::to_string(offsetY));

        _meshes.push_back(new Mesh(radius, { 0.f, 0.f, 0.0f }));
    }
}

void Tetromino::UpdateConstantBuffers(frt::Mesh::SceneObjectConstantBuffer* buffer)
{
    float radius = 1.0f, halfRadius = radius / 2.f;
    float offsetX = 0.f, offsetY = 0.f;
    XMFLOAT4X4 model, view, projection, mvp;

    for (int i = 0; i < 4; ++i)
    {
        offsetX = std::pow(-1.f, 1 * i) * -1.f;
        offsetY = std::pow(-1.f, i / 2);

        frt::Mesh::SceneObjectConstantBuffer buffer1 = *buffer;

        XMStoreFloat4x4(&model, XMMatrixMultiply(XMMatrixTranslation(offsetX, offsetY, 0.f),
                                                 XMMatrixRotationRollPitchYaw(_pitch, _yaw, _roll)));

        XMStoreFloat4x4(&view, _owner->GetWindow()->GetGraphics()._camera.GetViewMatrix());
        XMStoreFloat4x4(&projection, _owner->GetWindow()->GetGraphics()._camera.GetProjectionMatrix(0.8f, _owner->GetWindow()->GetGraphics()._aspectRatio));

        XMStoreFloat4x4(&mvp, XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection));

        XMStoreFloat4x4(&(buffer1.mvp), XMMatrixTranspose(XMLoadFloat4x4(&mvp)));
        XMStoreFloat4x4(&(buffer1.modelView), XMMatrixTranspose(XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view)));

        _meshes[i]->UpdateConstantBuffer(buffer1);
    }
}

void Tetromino::InitializeGraphicsResources(Graphics* graphics)
{
    for (Mesh* mesh : _meshes)
    {
        mesh->InitializeGraphicsResources(graphics);
    }
}

void Tetromino::InitializeConstantBuffers(Graphics* graphics)
{
    for (Mesh* mesh : _meshes)
    {
        mesh->InitializeConstantBuffers(graphics);
    }
}

void Tetromino::PopulateCommandList()
{
    for (Mesh* mesh : _meshes)
    {
        mesh->PopulateCommandList();
    }
}
