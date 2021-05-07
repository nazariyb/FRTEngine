#include "BoardBox.h"
#include <DirectXMath.h>


#include "App.h"
#include "TetrisBoard.h"
#include "Tetromino.h"
#include "Render/Primitives/Plane.h"

using namespace DirectX;

BoardBox::BoardBox()
{
    _plane = new frt::Plane(10.f, TetrisBoard::LeftBound);
}

BoardBox::~BoardBox()
{
    delete _plane;
}

void BoardBox::Update()
{
    UpdateConstantBuffers();
}

void BoardBox::PopulateCommandList()
{
    _plane->PopulateCommandList();
}

void BoardBox::InitializeGraphicsResources(frt::Graphics* graphics)
{
    _plane->InitializeGraphicsResources(graphics);
}

void BoardBox::InitializeConstantBuffers(frt::Graphics* graphics)
{
    _plane->InitializeConstantBuffers(graphics);
}

void BoardBox::UpdateConstantBuffers()
{
    XMFLOAT4X4 model, view, projection, mvp;
    XMFLOAT3 meshPosition = TetrisBoard::LeftBound;
    frt::Mesh::SceneObjectConstantBuffer buffer;

    // meshPosition = cell->mesh->GetWorldPosition();

    XMStoreFloat4x4(&model, XMMatrixTranslation(meshPosition.x, meshPosition.y, meshPosition.z));
    XMStoreFloat4x4(&view, frt::App::GetInstance()->GetWindow()->GetGraphics()._camera.GetViewMatrix());
    XMStoreFloat4x4(&projection,
                    frt::App::GetInstance()->GetWindow()->GetGraphics()._camera.GetProjectionMatrix(
                        0.8f, frt::App::GetInstance()->GetWindow()->GetGraphics()._aspectRatio));
    
    XMStoreFloat4x4(&mvp, XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection));

    memcpy(&buffer, &Tetromino::baseBuffer, sizeof(frt::Mesh::SceneObjectConstantBuffer));
    XMStoreFloat4x4(&buffer.mvp, XMMatrixTranspose(XMLoadFloat4x4(&mvp)));
    XMStoreFloat4x4(&buffer.modelView, XMMatrixTranspose(XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view)));

    _plane->UpdateConstantBuffer(buffer);
}
