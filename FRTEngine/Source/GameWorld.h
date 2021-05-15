#pragma once
#include "FRTEngine.h"
#include <vector>
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

    template<typename T, class ... Args>
    T* SpawnObject(Args&&... args);

    inline void DestroyObject(GameObject* object)
    {
        _gameObjects.erase(std::remove(_gameObjects.begin(), _gameObjects.end(), object), _gameObjects.end());
    }
    
    void InitializeGraphicsResources(class Graphics* graphics);

    virtual void Update() override;
    virtual void PopulateCommandList() override;
    void BeforeFirstTick();

protected:
    std::vector<GameObject*> _gameObjects;
};


template<typename T, class ... Args>
T* GameWorld::SpawnObject(Args&&... args)
{
    static_assert(std::derived_from<T, GameObject> == true, "templated type must be publicly derived from GameObject");

    T* newGameObject = new T(args...);

    _gameObjects.push_back(newGameObject);

    return newGameObject;
}

}

