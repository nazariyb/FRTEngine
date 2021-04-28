#pragma once
#include "FRTEngine.h"
#include <DirectXMath.h>
#include <vector>
#include <WindowsMinimal.h>
#include <memory>
#include "Render/Mesh.h"
#include "ITickable.h"
#include "GameObject.h"


namespace frt
{
class FRTENGINE_API GameWorld : public ITickable
{
public:
    GameWorld();
    ~GameWorld();

    template<typename T>
    T* SpawnObject();

    //void RegisterGameObject();
    //void Reserve(UINT GameObjectsNum);
    //const std::vector<DirectX::XMFLOAT4X4>& GetMeshes() const;
    //void RotateObject(UINT Index, float Roll, float Pitch, float Yaw);

    void InitializeGraphicsResources(class Graphics* graphics);

    virtual void Update() override {};
    virtual void PopulateCommandList() override;
    void BeforeFirstTick();

protected:
    //std::vector<DirectX::XMFLOAT4X4> _meshes;

    std::vector<GameObject*> _gameObjects;
};


template<typename T>
T* GameWorld::SpawnObject()
{
    static_assert(std::derived_from<T, GameObject> == true, "templated type must be publicly derived from GameObject");

    T* newGameObject = new T();

    _gameObjects.push_back(newGameObject);

    return newGameObject;
}

}

