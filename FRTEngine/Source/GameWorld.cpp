#include "GameWorld.h"
#include "Render/Mesh.h"
#include "App.h"
#include "Window.h"

namespace frt
{

GameWorld::GameWorld()
{}

GameWorld::~GameWorld()
{
    for (size_t i = 0; i < _gameObjects.size(); ++i)
    {
        delete _gameObjects.at(i);
    }
}

void GameWorld::Update()
{
    for (GameObject* gameObject : _gameObjects)
    {
        gameObject->Update();
    }
}

void GameWorld::InitializeGraphicsResources(Graphics* graphics)
{
    for (GameObject* gameObject : _gameObjects)
    {
        gameObject->InitializeGraphicsResources(graphics);
    }
}

void GameWorld::PopulateCommandList()
{
    for (GameObject* gameObject : _gameObjects)
    {
        gameObject->PopulateCommandList();
    }
}

void GameWorld::BeforeFirstTick()
{
    for (unsigned i = 0; i < Graphics::FrameCount; ++i)
    for (GameObject* gameObject : _gameObjects)
    {
        gameObject->InitializeConstantBuffers(&(App::GetInstance()->GetWindow()->GetGraphics()));
    }
}

}
