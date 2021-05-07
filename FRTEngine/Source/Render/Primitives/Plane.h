#pragma once
#include "FRTEngine.h"
#include "Render/Mesh.h"

namespace frt
{
class FRTENGINE_API Plane : public Mesh 
{
public:
    Plane();
    explicit Plane(float radius);
    Plane(float radius, DirectX::XMFLOAT3 initialPosition);
    virtual ~Plane();

    virtual void Resize(float newRadius) override;

    virtual void Update() override;
    virtual void PopulateCommandList() override;
};   
}
