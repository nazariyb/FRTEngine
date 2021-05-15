#pragma once
#include "FRTEngine.h"

#include <stdint.h>
#include <vector>
#include <functional>


namespace frt
{
class FRTENGINE_API Event
{
public:
    virtual ~Event() = default;
    virtual void Invoke() noexcept;

    virtual Event& operator+=(std::function<void(Event*)>);

    inline virtual bool IsValid() const noexcept = 0;

private:
    std::vector<std::function<void(Event*)>> listeners;
};
}

