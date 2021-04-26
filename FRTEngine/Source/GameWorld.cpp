#include "GameWorld.h"
#include "Render/Mesh.h"


namespace frt
{

GameWorld::GameWorld()
{

}

GameWorld::~GameWorld()
{

}

void GameWorld::RegisterGameObject()
{
    _meshes.emplace_back();
}

void GameWorld::Reserve(UINT GameObjectsNum)
{
    _meshes.resize(GameObjectsNum);
    for (UINT i = 0; i < GameObjectsNum; ++i)
    {
        DirectX::XMStoreFloat4x4(&_meshes[i], DirectX::XMMatrixIdentity());
    }
}

const std::vector<DirectX::XMFLOAT4X4>& GameWorld::GetMeshes() const
{
    return _meshes;
}

void GameWorld::RotateObject(UINT Index, float Roll, float Pitch, float Yaw)
{
    DirectX::XMStoreFloat4x4(&_meshes[Index],
                             DirectX::XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll));
}

void GameWorld::InitializeGraphicsResources(Graphics* graphics)
{
    for (Mesh* gameObject : _gameObjects)
    {
        gameObject->InitializeGraphicsResources(graphics);
    }
}

}
