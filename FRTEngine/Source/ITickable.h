#pragma once

#include "FRTEngine.h"

namespace frt
{
class FRTENGINE_API ITickable
{
public:
    virtual void Update() = 0;
    virtual void PopulateCommandList() = 0;
};
}

