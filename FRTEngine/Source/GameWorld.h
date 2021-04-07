#pragma once
#include "FRTEngine.h"
#include <DirectXMath.h>
#include <vector>
#include <WindowsMinimal.h>

namespace frt
{
class FRTENGINE_API GameWorld
{
public:
    GameWorld();
    ~GameWorld();

    void RegisterGameObject();
    void Reserve(UINT GameObjectsNum);
    const std::vector<DirectX::XMFLOAT4X4>& GetMeshes() const;
    void RotateObject(UINT Index, float Roll, float Pitch, float Yaw);

protected:
    std::vector<DirectX::XMFLOAT4X4> _meshes;

};
}

