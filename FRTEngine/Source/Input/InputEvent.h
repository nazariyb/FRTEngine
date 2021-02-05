#pragma once

#include "FRTEngine.h"
#include "Event.h"


namespace frt
{
class FRTENGINE_API InputEvent : public Event
{
public:
    InputEvent() = default;

    inline virtual bool IsPress() const noexcept = 0;
    inline virtual bool IsRelease() const noexcept = 0;

    inline virtual bool IsValid() const noexcept override = 0;
};
}

#define INPUT_EVENT_TYPE_INIT()\
Invalid,\
Press,\
Release,
