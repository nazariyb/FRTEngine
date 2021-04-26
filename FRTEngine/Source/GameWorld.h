#pragma once
#include "FRTEngine.h"
#include <DirectXMath.h>
#include <vector>
#include <WindowsMinimal.h>
#include <memory>
#include "Render/Mesh.h"


namespace frt
{
class FRTENGINE_API GameWorld
{
public:
    GameWorld();
    ~GameWorld();

    template<typename T>
    T* SpawnObject();

    void RegisterGameObject();
    void Reserve(UINT GameObjectsNum);
    const std::vector<DirectX::XMFLOAT4X4>& GetMeshes() const;
    void RotateObject(UINT Index, float Roll, float Pitch, float Yaw);

    void InitializeGraphicsResources(class Graphics* graphics);

    inline Mesh* GetFirst()
    {
        return _gameObjects.front();
    }


protected:
    std::vector<DirectX::XMFLOAT4X4> _meshes;

    std::vector<Mesh*> _gameObjects;
};


template<typename T>
T* GameWorld::SpawnObject()
{
    static_assert(std::derived_from<T, Mesh> == true, "templated type must be publicly derived from Mesh");

    Mesh* newGameObject = new T();

    _gameObjects.push_back(newGameObject);

    return newGameObject;
}

}

