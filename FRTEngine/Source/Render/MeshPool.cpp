#include "MeshPool.h"
#include "Render/Mesh.h"

namespace frt
{
MeshPool::MeshPool(unsigned int meshesNum)
    : _meshesNum(meshesNum)
    , _meshes(meshesNum, nullptr)
    , _meshUsageFlags(meshesNum, false)
{
    for (unsigned int i = 0; i < _meshesNum; ++i)
    {
        _meshes[i] = new Mesh(0.f);
    }
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
    for (Mesh* mesh : _meshes)
    {
        mesh->PopulateCommandList();
    }
}

}
