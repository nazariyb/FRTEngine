#include "MeshPool.h"

#include "Primitives/Cube.h"
#include "Render/Mesh.h"
#include "Utils/Logger/Logger.h"

namespace frt
{
MeshPool::MeshPool(unsigned int meshesNum)
    : _meshesNum(meshesNum)
    , _meshes(meshesNum, nullptr)
    , _meshUsageFlags(meshesNum, false)
{
    for (unsigned int i = 0; i < _meshesNum; ++i)
    {
        _meshes[i] = new Cube(0.f);
    }
    Logger::DebugLogInfo("Meshes created: " + std::to_string(_meshes.size()));
}

MeshPool::~MeshPool()
{
    for (unsigned int i = 0; i < _meshesNum; ++i)
    {
        delete _meshes.at(i);
    }
}

std::pair<MeshPool::Result, std::vector<Mesh*>> MeshPool::GetFreeMeshes(unsigned int amount)
{
    std::vector<Mesh*> meshes;
    meshes.reserve(amount);

    for (unsigned int i = 0; i < _meshesNum; ++i)
    {
        if (!_meshUsageFlags[i])
        {
            meshes.push_back(_meshes[i]);
            _meshUsageFlags[i] = true;
            if (meshes.size() == amount) break;
        }
    }

    return
    {
        meshes.size() == amount ? Result::Ok : Result::NotEnoughFreeMeshes,
        meshes
    };
}

void MeshPool::ReleaseMesh(Mesh* mesh)
{
    for (unsigned i = 0; i < _meshes.size(); ++i)
    {
        if (_meshes[i] == mesh)
        {
            _meshUsageFlags[i] = false;
            Logger::DebugLogInfo("Mesh Released");
            break;
        }
    }
}

void MeshPool::InitializeGraphicsResources(class Graphics* graphics)
{
    for (Mesh* mesh : _meshes)
    {
        mesh->InitializeGraphicsResources(graphics);
    }
}

void MeshPool::InitializeConstantBuffers(class Graphics* graphics)
{
    for (Mesh* mesh : _meshes)
    {
        mesh->InitializeConstantBuffers(graphics);
    }
}

void MeshPool::PopulateCommandList()
{
    for (unsigned i = 0; i < _meshes.size(); ++i)
    {
        if (_meshUsageFlags[i])
            _meshes[i]->PopulateCommandList();
    }
}

}
