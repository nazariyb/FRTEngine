#pragma once
#include "FRTEngine.h"
#include "Render/Mesh.h"

namespace frt
{
class FRTENGINE_API Plane : public Mesh 
{
public:
    enum Orientation
    {
        X, Y, Z
    };

    Plane() = delete;
    Plane(float radius) = delete;
    Plane(float width, float height, DirectX::XMFLOAT3 initialPosition, Orientation orientation);
    virtual ~Plane();
    
    virtual void Resize(float newRadius) override;

    virtual void Update() override;
    virtual void PopulateCommandList() override;

private:
    Orientation _orientation;
    float _width;
    float _height;
};   
}
