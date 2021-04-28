#pragma once

#include "FRTEngine.h"

namespace frt
{
class FRTENGINE_API GameObject
{
public:
    virtual void InitializeGraphicsResources(class Graphics* graphics) {};
    virtual void InitializeConstantBuffers(class Graphics* graphics) {};
    virtual void PopulateCommandList() {};
};
}

