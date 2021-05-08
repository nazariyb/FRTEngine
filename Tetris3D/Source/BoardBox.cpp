#include "BoardBox.h"
#include <DirectXMath.h>


#include "App.h"
#include "TetrisBoard.h"
#include "Tetromino.h"
#include "Render/Primitives/Plane.h"

using namespace DirectX;

BoardBox::BoardBox()
{
    _leftBorder = new frt::Plane(2.f, 40.f,
                                 {0.f, -1.f, 0.f},
                                 frt::Plane::X);
    _rightBorder = new frt::Plane(2.f, 40.f,
                                  {0.f, -1.f, 0.f},
                                  frt::Plane::X);
    _floor = new frt::Plane(150.f, 150.f,
                            {0.f, -1.f, 0.f},
                            frt::Plane::Y);
}

BoardBox::~BoardBox()
{
    delete _leftBorder;
    delete _rightBorder;
    delete _floor;
}

void BoardBox::Update()
{
    UpdateConstantBuffers();
}

void BoardBox::PopulateCommandList()
{
    _leftBorder->PopulateCommandList();
    _rightBorder->PopulateCommandList();
    _floor->PopulateCommandList();
}

void BoardBox::InitializeGraphicsResources(frt::Graphics* graphics)
{
    _leftBorder->InitializeGraphicsResources(graphics);
    _rightBorder->InitializeGraphicsResources(graphics);
    _floor->InitializeGraphicsResources(graphics);
}

void BoardBox::InitializeConstantBuffers(frt::Graphics* graphics)
{
    _leftBorder->InitializeConstantBuffers(graphics);
    _rightBorder->InitializeConstantBuffers(graphics);
    _floor->InitializeConstantBuffers(graphics);
}

void BoardBox::UpdateConstantBuffers()
{
    XMFLOAT4X4 model, view, projection, mvp;
    frt::Mesh::SceneObjectConstantBuffer buffer;

    XMFLOAT3 meshPosition = TetrisBoard::LeftBound;

    // meshPosition = cell->mesh->GetWorldPosition();

    XMStoreFloat4x4(&model, XMMatrixTranslation(meshPosition.x - 1.f, meshPosition.y, meshPosition.z));
    XMStoreFloat4x4(&view, frt::App::GetInstance()->GetWindow()->GetGraphics()._camera.GetViewMatrix());
    XMStoreFloat4x4(&projection,
                    frt::App::GetInstance()->GetWindow()->GetGraphics()._camera.GetProjectionMatrix(
                        1.f, frt::App::GetInstance()->GetWindow()->GetGraphics()._aspectRatio));

    XMStoreFloat4x4(&mvp, XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection));

    memcpy(&buffer, &Tetromino::baseBuffer, sizeof(frt::Mesh::SceneObjectConstantBuffer));
    // XMStoreFloat4x4(&buffer.mvp, XMMatrixTranspose(XMLoadFloat4x4(&mvp)));
    // XMStoreFloat4x4(&buffer.modelView, XMMatrixTranspose(XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view)));
    XMStoreFloat4x4(&buffer.model, XMMatrixTranspose(XMLoadFloat4x4(&model)));
    XMStoreFloat4x4(&buffer.viewProj, XMMatrixTranspose(XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection)));
    
    _leftBorder->UpdateConstantBuffer(buffer);

    meshPosition = TetrisBoard::RightBound;

    XMStoreFloat4x4(&model, XMMatrixTranslation(meshPosition.x + 1.f, meshPosition.y, meshPosition.z));

    XMStoreFloat4x4(&mvp, XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection));

    // XMStoreFloat4x4(&buffer.mvp, XMMatrixTranspose(XMLoadFloat4x4(&mvp)));
    // XMStoreFloat4x4(&buffer.modelView, XMMatrixTranspose(XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view)));
    XMStoreFloat4x4(&buffer.model, XMMatrixTranspose(XMLoadFloat4x4(&model)));
    XMStoreFloat4x4(&buffer.viewProj, XMMatrixTranspose(XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection)));
    
    _rightBorder->UpdateConstantBuffer(buffer);

    meshPosition = TetrisBoard::BottomBound;

    XMStoreFloat4x4(&model, XMMatrixTranslation(meshPosition.x, meshPosition.y, meshPosition.z));

    XMStoreFloat4x4(&mvp, XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection));

    // XMStoreFloat4x4(&buffer.mvp, XMMatrixTranspose(XMLoadFloat4x4(&mvp)));
    // XMStoreFloat4x4(&buffer.modelView, XMMatrixTranspose(XMLoadFloat4x4(&model) * XMLoadFloat4x4(&view)));
    XMStoreFloat4x4(&buffer.model, XMMatrixTranspose(XMLoadFloat4x4(&model)));
    XMStoreFloat4x4(&buffer.viewProj, XMMatrixTranspose(XMLoadFloat4x4(&view) * XMLoadFloat4x4(&projection)));
    
    _floor->UpdateConstantBuffer(buffer);
}
