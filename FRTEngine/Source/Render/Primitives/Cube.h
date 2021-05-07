#pragma once
#include "FRTEngine.h"
#include "Render/Mesh.h"


namespace frt
{
class FRTENGINE_API Cube : public Mesh
{
public:
    Cube();
    explicit Cube(float radius);
    Cube(float radius, DirectX::XMFLOAT3 initialPosition);
    virtual ~Cube();

    virtual void Resize(float newRadius) override;

    virtual void Update() override;
    virtual void PopulateCommandList() override;
    
};    
}
