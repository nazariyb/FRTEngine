#pragma once
#include "FRTEngine.h"
#include <vector>
#include "GameObject.h"

namespace frt
{
class FRTENGINE_API MeshPool : public GameObject
{
public:
    enum class Result
    {
        Ok,
        NotEnoughFreeMeshes
    };

public:
    MeshPool() = delete;
    MeshPool(const MeshPool&) = delete;
    MeshPool(MeshPool&&) = delete;

    MeshPool(unsigned int meshesNum);
    ~MeshPool();

    std::pair<Result, std::vector<class Mesh*>> GetFreeMeshes(unsigned int amount);
    void ReleaseMesh(Mesh* mesh);
    
    virtual void InitializeGraphicsResources(class Graphics* graphics) override;
    virtual void InitializeConstantBuffers(class Graphics* graphics) override;
    virtual void PopulateCommandList() override;

private:
    unsigned int _meshesNum;
    std::vector<class Mesh*> _meshes;
    std::vector<bool> _meshUsageFlags;
};
}

