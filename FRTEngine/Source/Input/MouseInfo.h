#pragma once
#include "FRTEngine.h"
#include "WindowsMinimal.h"

#include <stdint.h>


namespace frt
{
enum class FRTENGINE_API MouseButtonType : uint8_t
{
    None,
    Left,
    Right,
    Middle,
    Other
};

class FRTENGINE_API MouseState
{
    friend class Mouse;
    friend class MouseEvent;

public:

    MouseState() noexcept;
    MouseState(uint8_t buttonNumber, POINTS pointerPosition) noexcept;

private:

    MouseButtonType buttonType;
    uint8_t buttonNumber;
    POINTS pointerPosition;
};
}