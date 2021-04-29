#pragma once

#include "FRTEngine.h"
#include "ITickable.h"

namespace frt
{
class FRTENGINE_API GameObject : public ITickable
{
public:
    virtual void InitializeGraphicsResources(class Graphics* graphics) {};
    virtual void InitializeConstantBuffers(class Graphics* graphics) {};

public:
    virtual void Update() override {};
    virtual void PopulateCommandList() override {};
};
}

